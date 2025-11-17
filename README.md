#include <stdio.h>
#include <string.h>

struct cliente{
	 int cpf;
	 int contato;
	 int compras;
	 char nome[50];
	 char email[50];
};

int conferirCpf(char* Ccpf){
    int aux,n=10000000000,vetorCpf[11];
	for(int i=0;i<11;i++){
		aux=Ccpf->cpf/n;
		vetorCpf[i]=aux;
		aux=Ccpf->cpf%n;
		n=/10;
	}
}
int conferirEmail(char* Eemail){
	char gmail[11]='@gmail.com';
	char hotmail[13]='@hotmail.com';
	int retorno;
	int count=0;
	int tam=strlen(Eemail->email);
	while(count<tam+1){
		if(Eemail->email[count]=='@';){
			retorno=strncmp(Eemail->email[count],gmail);
				if(retorno==0){
					break;
				}
			else{
				retorno=strncmp(Eemail->email[count],hotmail);
					if(retorno==0){
						break;
					}
			}
		}
	}
	return retorno;
}

void cadastro (int* P){
int controle=1;

    printf("Nome do cliente: \n");
	fgets(P->nome,50,stdin);
	
	while(controle==1){ //verifica se o cpf tem 11 dígitos//
		printf("Cpf do cliente: \n");
		scanf("%i",P->cpf);
		controle=conferir(P->cpf);
	} 
	
	controle=1;
	
	printf("Contato do cliente: \n");
	scanf("%i",P->contato);
	
	 while(controle==1){
		printf("E-mail do cliente: \n");
		fgets(P->email,50,stdin);
		controle=conferirEmail(P->email);
	}
		
	 printf("Quantidade de compras do cliente: \n");
	 scanf("%i",P->compras);
}

int main(){
	 struct cliente pessoa;
	 int escolha=-1;
	 char yn[2];
	 FILE* arqPessoa;
	 arqPessoa=fopen("C:\\Área de trabalho\\clientes.txt","w");
	 if(arqPessoa==NULL){
		  printf("Erro na abertura do arquivo");
		  return 1;
	 }
	 printf("\n\n=========MENU=========\n");
	 while(escolha!=0){
		  printf("Digite o número correspondente a opção que deseja.\n");
		  printf("1) Cadastrar um cliente.\n");
		  
		  scanf("%i",&escolha);
		  switch(escolha)
		  case 1:
				while(yn[0]!='N' || yn[0]!='n'){
					 cadastro(&pessoa);
					 fwrite(&pessoa,sizeof(struct pessoa),1,arqPessoa);
					 printf("Deseja fazer um novo cadastro?[S/N]");
					 scanf("%c",yn);
				}
	 }
}
