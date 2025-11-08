#include<stdio.h>
#include<string.h>
#include<stdlib.h>

typedef struct
{
    char cpf[12];
    char nome[100];
    char telefone[12];
    char email[100];
    float limite_compra;
} Cliente;

void alterar_cliente()
{
    FILE *arquivo;
    arquivo=fopen("C:\\Users\\nic\\OneDrive\\Desktop\\UEG\\CRUD\\src\\output\\clientes.txt","r+");
    if(arquivo==NULL)
    {
        arquivo=fopen("C:\\Users\\nic\\OneDrive\\Desktop\\UEG\\CRUD\\src\\output\\clientes.txt","w+");
        if(arquivo==NULL)
        {
            printf("Erro na abertura do arquivo.\n");
            return;
        }
    }

    Cliente cliente;
    char buscar_cpf[12];
    int encontrado=0;
    long posicao;

    printf("\n-----ALTERAR CLIENTE-----\n");
    printf("Insira o CPF:");
    scanf("%s",buscar_cpf);

    while(fread(&cliente,sizeof(Cliente),1,arquivo)==1)
    {
        if(strcmp(cliente.cpf,buscar_cpf)==0)
        {
            encontrado=1;
            posicao=ftell(arquivo)-sizeof(Cliente);

            printf("\nCliente encontrado!\n");
            printf("CPF: %s\n",cliente.cpf);
            printf("Nome atual: %s\n",cliente.nome);
            printf("Telefone atual: %s\n",cliente.telefone);
            printf("Email atual: %s\n",cliente.email);
            printf("Limite atual: %.2f\n",cliente.limite_compra);

            printf("\nInforme os novos dados\n");
            printf("Novo nome: ");
            scanf(" %s",cliente.nome);
            printf("Novo telefone: ");
            scanf(" %s",cliente.telefone);
            printf("Novo email: ");
            scanf(" %s",cliente.email);
            printf("Novo limite de compra: ");
            scanf("%f",&cliente.limite_compra);

            fseek(arquivo,posicao,SEEK_SET);
            fwrite(&cliente,sizeof(Cliente),1,arquivo);

            printf("\nRegistro de cliente alterado com sucesso!\n");
            break;
        }
    }

    if(!encontrado)
    {
        printf("\nCliente não encontrado.\n");
    }

    fclose(arquivo);
}

int main()
{
    alterar_cliente();
    return 0;
}

