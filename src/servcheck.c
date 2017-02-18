#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <fcntl.h>
#include <arpa/inet.h>

int sockfd, newsockfd, client_len, portnum, bytes, n, c, offset_size, chunk_size, err, size_check, bytes_read;
int file_size = 0;
int option = 1, file_avail = 1, exitconn = 1;
char send_buff[256], recev_buff[256], buffer[256], name[256];
struct sockaddr_in server_addr, client_addr;
struct stat s = {0};
FILE *file_pointer;

void werr(int bytes)
{
	if( bytes == -1 )
     	printf(" Writing error.\n");
}

int estab_connection( int argc, char *argv[] )
{
	// listening for clients:
	printf("Waiting for client\n");
	listen( sockfd, 10 );
    client_len = sizeof( client_addr );

    //Accepting connection from client 
    newsockfd = accept( sockfd, (struct sockaddr *) &client_addr, &client_len );
    werr(newsockfd);
    if ( newsockfd < 0 ) exit(1);
	
	printf("Connected to Client\n");
    return newsockfd;
}

int check_request( int newsockfd )
{
	if( (n = read( newsockfd, recev_buff, 256 ) ) != 0)
    {
		if(strstr( recev_buff, "FILE-CHECK" ) != NULL)
		{
        	option = 1;		//Setting option flag as 1 i.e. file size is requested
	    	bzero( recev_buff, 256 );    
            bzero( send_buff, 256 );
            strcpy( send_buff, "ACK" );
            n =  write(newsockfd, send_buff, strlen(send_buff));
	    werr( n );
		}
        else
        {
			if( strstr( recev_buff, "GET-DATA" ) != NULL )
          	{
          		option = 2; //Here data is requested
             	bzero( recev_buff, 256 );                                      
            	bzero( send_buff, 256 );
            	strcpy( send_buff, "ACK" );
            	n =  write( newsockfd, send_buff, strlen( send_buff ) );
            	werr ( n );
        	}
		}
  }
  return option;
}


int respond( int option )
{
	char *strlocation;
	if( option == 1 )
    {
		if( ( n = read( newsockfd, recev_buff, 256 ) ) != 0 )
		{
            strlocation = strstr( recev_buff, "End of file" );
        	printf("\n The file name sent by Client is: %s", recev_buff );  

		}

        // Check whether file exists

        if( access( recev_buff, F_OK ) != -1 )
        	file_avail= 1;
        else 
        	file_avail = 0;

        // Obtain file size
        bzero( send_buff,256 );  
            
        if( file_avail == 1 )
        {      
        	stat( recev_buff, &s );
        	file_size = s.st_size;
        	sprintf( send_buff, "%d", file_size );
       }
       else
       		strcpy( send_buff, "NO FILE" );

            // Sending file size or NO FILE message
        n =  write( newsockfd, send_buff, strlen( send_buff ) );
	werr( n );        
     }
            
	else if( option == 2 )
    {
		bzero( recev_buff,256 );  

          //Reading the file name
  		if( (n = read(newsockfd, recev_buff, 256)) != 0)
  		{
        	strlocation = strstr( recev_buff, "End of file" );
        	strcpy( name,recev_buff );
      	}

		// Send ACK for name

      bzero( send_buff, 256 );
      strcpy( send_buff, "ACK" );
      n =  write( newsockfd, send_buff, strlen( send_buff ) );
      werr( n );

          // Read Offset Size
    while( (n = read( newsockfd, buffer, 256 ) ) != 0 )
    {
	    if( strstr( buffer, "ACK" ) != NULL )
             break;
        else
           offset_size = atoi( buffer );
  	}
          	
          // Send ACK for Offset_Size
  	bzero( send_buff, 256 );
    strcpy( send_buff, "ACK" );
    n =  write( newsockfd, send_buff, strlen( send_buff ) );
    werr( n );

      // Read chunk_size
	while( (n = read( newsockfd, buffer, 256 ) ) != 0)
	{
        if( strstr(buffer, "ACK") != NULL )
             break;
        else
           chunk_size = atoi( buffer );
            
	}
        
    printf("\n The offset size received is: %d",offset_size);
    printf("\n The chunk size received is: %d",chunk_size);
        
            // Send the Data

    file_pointer = fopen( name, "r" );
	if ( file_pointer == NULL )
		{
            printf("\nERROR: Could not open File");
            return 1;   
    	}  

    n = fseek( file_pointer, offset_size, SEEK_SET );
    werr ( n );
    size_check = chunk_size/256;   // Calculating loop iterations
            
    bzero( send_buff,256 );

    n = 0;

    while (n != ( size_check+1 ) ) 
    {
		bytes_read = fread( send_buff, 1, 256, file_pointer );
		if( bytes_read > 0 )
		{
            write( newsockfd, send_buff, bytes_read );
        }
	
	    if ( bytes_read < 256 )
        {
	    	if ( feof( file_pointer ) )
            	printf("End of file\n");
        	if (ferror( file_pointer ) )
            	printf("ERROR: Could not read file. \n");
            break;
        }
        n++;
  }

   bzero( send_buff,256 );
   strcpy( send_buff,"ENDOFFILE" );
       
   n =  write( newsockfd, send_buff, strlen( send_buff ) );
       werr ( n );

     }	
     
   return 0;
		
}

void main( int argc, char *argv[] )
{
    	
	if ( argc != 2 )  //Check for valid input
	{
      printf("usage %s port\n", argv[0]);
      exit(0);
   	}
   	
   		portnum = atoi( argv[1] ); 
		
		sockfd = socket( AF_INET, SOCK_STREAM, 0 ); 

		werr(sockfd);

		// Initialize socket
		
		bzero( (char *) &server_addr, sizeof(server_addr) );

		server_addr.sin_family = AF_INET;
		server_addr.sin_addr.s_addr = INADDR_ANY;
		server_addr.sin_port = htons( portnum );

		bytes = bind( sockfd, ( struct sockaddr * ) &server_addr, sizeof( server_addr ) );
		werr( bytes );
		if ( bytes < 0 ) 
			exit(0);
   
   		while( 1 )
		{
			newsockfd = estab_connection( argc, argv );
			option=check_request( newsockfd );
			respond( option );
			close( newsockfd );
		}
}

