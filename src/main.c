#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

typedef struct
{
    char cpf[12];
    char nome[100];
    char contato[20];
    char email[100];
    float limite_compra;
    int ativo;
    int compras;
} Cliente;

void clear_input_buffer(void) {
    int c;
    while ((c=getchar())!='\n' && c!=EOF) { }
}

//Validar CPF
int conferir_formato_Cpf(const char *cpf) {
    if (cpf==NULL) return 0;

    char d[12];
    int j=0;
    for (int i=0; cpf[i]!='\0' && j<11; ++i) {
        if (isdigit((unsigned char)cpf[i])) {
            d[j++]=cpf[i];
        }
    }

    if (j!=11) return 0;
    d[11]='\0';

    int todosIguais=1;
    for (int i=1; i<11; ++i) {
        if (d[i]!=d[0]) { todosIguais=0; break; }
    }
    if (todosIguais) return 0;

    int soma=0;
    for (int i=0; i<9; ++i) soma+=(d[i]-'0')*(10-i);
    int resto=soma%11;
    int dig1=(resto<2)?0:11-resto;
    if (dig1!=(d[9]-'0')) return 0;

    soma=0;
    for (int i=0; i<10; ++i) soma+=(d[i]-'0')*(11-i);
    resto=soma%11;
    int dig2=(resto<2)?0:11-resto;
    if (dig2!=(d[10]-'0')) return 0;

    return 1;
}

//Conferir duplicidade CPF
int conferirCpf(char Ccpf[], FILE *arqPessoa)
{
    if (arqPessoa==NULL) {
        return 1;
    }

    Cliente temp;
    rewind(arqPessoa);
    while (fread(&temp, sizeof(Cliente), 1, arqPessoa)==1) {
        if (temp.ativo && strcmp(temp.cpf, Ccpf)==0) {
            printf("CPF ja cadastrado.\n");
            return 0;
        }
    }
    return 1;
}

//EMAIL
int conferirEmail(char Eemail[])
{
    if (Eemail==NULL){
        return 0;
    }

    if (strstr(Eemail, "@gmail.com") || strstr(Eemail, "@hotmail.com") || strstr(Eemail, "@outlook.com")) {
        return 1;
    }
    printf("Email invalido! Aceito apenas @gmail.com, @hotmail.com ou @outlook.com.\n");
    return 0;
}

//FUNCOES
//CADASTRAR CLIENTE
void cadastro(Cliente *P, int *qtd, FILE *arqPessoa) {
    int valido=0;
    clear_input_buffer();

    printf("\nCADASTRO DE CLIENTE\n");
    printf("Nome do cliente: ");
    fgets(P->nome, sizeof(P->nome), stdin);
    P->nome[strcspn(P->nome, "\n")]='\0';

    valido=0;
    while (!valido) {
        printf("CPF (11 digitos): ");
        fgets(P->cpf, sizeof(P->cpf), stdin);
        P->cpf[strcspn(P->cpf, "\n")]='\0';

        if (conferir_formato_Cpf(P->cpf) && conferirCpf(P->cpf, arqPessoa)) {
            valido=1;
        } else {
            printf("CPF invalido!\n");
        }
    }

    clear_input_buffer();
    printf("Contato (telefone): ");
    fgets(P->contato, sizeof(P->contato), stdin);
    P->contato[strcspn(P->contato, "\n")]='\0';

    valido=0;
    while (!valido) {
        printf("Email: ");
        fgets(P->email, sizeof(P->email), stdin);
        P->email[strcspn(P->email, "\n")]='\0';

        if (conferirEmail(P->email))
            valido=1;
    }

    printf("Qual o limite de compra desejado? (ex: 1500.50) ");
    while (scanf("%f", &P->limite_compra)!=1) {
        printf("Entrada invalida. Digite um numero valido: ");
        clear_input_buffer();
    }

    if (arqPessoa!=NULL) {
        fseek(arqPessoa, 0, SEEK_END);
        fwrite(P, sizeof(Cliente), 1, arqPessoa);
        fflush(arqPessoa);
    }

    if (qtd!=NULL) (*qtd)++;
}

//LISTAR CLIENTES
void listarClientes(FILE *arqPessoa) {
    if (arqPessoa==NULL) {
        printf("Arquivo de clientes nao esta aberto.\n");
        return;
    }

    Cliente temp;

    rewind(arqPessoa);

    printf("\nCLIENTES CADASTRADOS\n");

    while (fread(&temp, sizeof(Cliente), 1, arqPessoa)==1) {
        if (!temp.ativo) continue;
        printf("Nome: %s\n", temp.nome);
        printf("CPF: %s\n", temp.cpf);
        printf("Contato: %s\n", temp.contato);
        printf("Email: %s\n", temp.email);
        printf("Limite de compra: %.2f\n\n", temp.limite_compra);
    }
}

//BUSCAR CLIENTE
void buscarClientePorCPF(FILE *arqPessoa) {
    char cpfBusca[12];
    Cliente temp;
    int encontrado=0;

    if (arqPessoa==NULL) {
        printf("Arquivo de clientes nao esta aberto.\n");
        return;
    }

    printf("\nDigite o CPF para buscar: ");
    clear_input_buffer();
    fgets(cpfBusca, sizeof(cpfBusca), stdin);
    cpfBusca[strcspn(cpfBusca, "\n")]='\0';

    if (strlen(cpfBusca)!=11) {
        printf("CPF deve ter 11 digitos!\n");
        return;
    }

    rewind(arqPessoa);
    while (fread(&temp, sizeof(Cliente), 1, arqPessoa)==1) {
        if (!temp.ativo) continue;
        if (strcmp(temp.cpf, cpfBusca)==0) {
            encontrado=1;

            printf("\nCLIENTE ENCONTRADO\n");
            printf("Nome: %s\n", temp.nome);
            printf("CPF: %s\n", temp.cpf);
            printf("Contato: %s\n", temp.contato);
            printf("Email: %s\n", temp.email);
            printf("Limite de compra: %.2f\n", temp.limite_compra);
            break;
        }
    }

    if (!encontrado)
        printf("Nenhum cliente encontrado com esse CPF.\n");
}

//ALTERAR CLIENTE
void alterar_cliente()
{
    FILE *arquivo;
    arquivo=fopen("clientes.bin", "r+b");
    if (arquivo==NULL)
    {
        arquivo=fopen("clientes.bin", "w+b");
        if (arquivo==NULL)
        {
            printf("Erro na abertura do arquivo.\n");
            return;
        }
    }

    Cliente cliente;
    char buscar_cpf[12];
    int encontrado=0;
    long posicao;

    printf("\nALTERAR CLIENTE\n");
    printf("Insira o CPF:\n");
    clear_input_buffer();
    fgets(buscar_cpf, sizeof(buscar_cpf), stdin);
    buscar_cpf[strcspn(buscar_cpf, "\n")]='\0';

    if (strlen(buscar_cpf)!=11) {
        printf("CPF invalido (deve ter 11 digitos).\n");
        fclose(arquivo);
        return;
    }

    rewind(arquivo);
    while (fread(&cliente, sizeof(Cliente), 1, arquivo)==1)
    {
        if (!cliente.ativo) continue;
        if (strcmp(cliente.cpf, buscar_cpf)==0)
        {
            encontrado=1;
            posicao=ftell(arquivo)-sizeof(Cliente);

            printf("\nCliente encontrado!\n");
            printf("CPF: %s\n", cliente.cpf);
            printf("Nome atual: %s\n", cliente.nome);
            printf("Telefone atual: %s\n", cliente.contato);
            printf("Email atual: %s\n", cliente.email);
            printf("Limite atual: %.2f\n", cliente.limite_compra);

            printf("\nInforme os novos dados\n");

            printf("Novo nome: ");
            clear_input_buffer();
            char buffer[200];
            fgets(buffer, sizeof(buffer), stdin);
            buffer[strcspn(buffer, "\n")]='\0';
            if (strlen(buffer)>0) {
                strncpy(cliente.nome, buffer, sizeof(cliente.nome)-1);
                cliente.nome[sizeof(cliente.nome)-1]='\0';
            }

            printf("Novo telefone: ");
            fgets(buffer, sizeof(buffer), stdin);
            buffer[strcspn(buffer, "\n")]='\0';
            if (strlen(buffer)>0) {
                strncpy(cliente.contato, buffer, sizeof(cliente.contato)-1);
                cliente.contato[sizeof(cliente.contato)-1]='\0';
            }

            printf("Novo email: ");
            fgets(buffer, sizeof(buffer), stdin);
            buffer[strcspn(buffer, "\n")]='\0';
            if (strlen(buffer)>0) {
                if (conferirEmail(buffer)) {
                    strncpy(cliente.email, buffer, sizeof(cliente.email)-1);
                    cliente.email[sizeof(cliente.email)-1]='\0';
                } else {
                    printf("Email invalido. Mantendo email antigo.\n");
                }
            }

            printf("Novo limite de compra (digite apenas Enter para manter %.2f): ", cliente.limite_compra);
            fgets(buffer, sizeof(buffer), stdin);
            buffer[strcspn(buffer, "\n")]='\0';
            if (strlen(buffer)>0) {
                float novoLim;
                if (sscanf(buffer, "%f", &novoLim)==1) {
                    cliente.limite_compra=novoLim;
                } else {
                    printf("Entrada invalida para limite. Mantendo o anterior.\n");
                }
            }

            fseek(arquivo, posicao, SEEK_SET);
            fwrite(&cliente, sizeof(Cliente), 1, arquivo);
            fflush(arquivo);

            printf("\nRegistro de cliente alterado com sucesso!\n");
            break;
        }
    }

    if (!encontrado)
    {
        printf("\nCliente nao encontrado!\n");
    }

    fclose(arquivo);
}

//EXCLUIR
void excluir_cliente(){
    FILE *arquivo;
    arquivo=fopen("clientes.bin", "r+b");
    if (arquivo==NULL)
    {
        arquivo=fopen("clientes.bin", "w+b");
        if (arquivo==NULL)
        {
            printf("Erro na abertura do arquivo.\n");
            return;
        }
    }

    Cliente cliente;
    char buscar_cpf[12];
    int encontrado=0;
    long posicao;

    printf("\nEXCLUIR CLIENTE\n");
    printf("Insira o CPF:\n");
    clear_input_buffer();
    fgets(buscar_cpf, sizeof(buscar_cpf), stdin);
    buscar_cpf[strcspn(buscar_cpf, "\n")]='\0';

    if (strlen(buscar_cpf)!=11) {
        printf("CPF invalido (deve ter 11 digitos).\n");
        fclose(arquivo);
        return;
    }

    rewind(arquivo);
    while (fread(&cliente, sizeof(Cliente), 1, arquivo)==1) {
        if (!cliente.ativo) continue;
        if (strcmp(cliente.cpf, buscar_cpf)==0) {
            encontrado=1;
            posicao=ftell(arquivo)-sizeof(Cliente);

            cliente.ativo=0;
            fseek(arquivo, posicao, SEEK_SET);
            fwrite(&cliente, sizeof(Cliente), 1, arquivo);
            fflush(arquivo);
            printf("Cliente excluido!\n");
            break;
        }
    }

    if (!encontrado) printf("Cliente nao encontrado!\n");
    fclose(arquivo);
}

//MAIN
int main()
{
    int opcao;
    char yn='S';
    Cliente pessoa;
    int contador=0;
    FILE *arqPessoa=fopen("clientes.bin", "r+b");
    if (arqPessoa==NULL) {
        arqPessoa=fopen("clientes.bin", "w+b");
        if (arqPessoa==NULL) {
            printf("Erro ao criar/abrir arquivo de clientes.\n");
            return 1;
        }
    }

    do {
        printf("\n----------MENU------------\n");
        printf("(1) Cadastrar cliente\n");
        printf("(2) Consultar todos os clientes cadastrados\n");
        printf("(3) Pesquisar dados de um cliente especifico\n");
        printf("(4) Alterar cadastro de cliente\n");
        printf("(5) Excluir cliente cadastrado\n");
        printf("(0) Sair\n");
        printf("---------------------------\n");
        printf("Qual area deseja acessar? ");

        if (scanf("%d", &opcao)!=1) {
            printf("Entrada invalida. Digite um numero.\n");
            clear_input_buffer();
            continue;
        }

        switch (opcao)
        {
        case 1:
            yn='S';
            while (yn=='S' || yn=='s') {
                cadastro(&pessoa, &contador, arqPessoa);

                printf("Cadastrar outro cliente? (S/N): ");
                clear_input_buffer();
                yn=getchar();
                clear_input_buffer();
            }
            break;

        case 2:
            listarClientes(arqPessoa);
            break;

        case 3:
            buscarClientePorCPF(arqPessoa);
            break;

        case 4:
            alterar_cliente();
            break;

        case 5:
            excluir_cliente();
            break;

        case 0:
            printf("Saindo...\n");
            break;

        default:
            printf("Opcao invalida!\n");
            break;
        }
    } while (opcao!=0);

    fclose(arqPessoa);
    return 0;
}