#include <stdio.h>
#include <string.h>
#include <ctype.h>
struct cliente {
    char cpf[12];      
    char nome[50];
    char email[50];
    char contato[20];
    int compras;
};
int conferirCpf(char *cpfEntrada, FILE *arqPessoa) {
    if (strlen(cpfEntrada) != 11) {
        printf("CPF deve conter 11 dígitos!\n");
        return 0;
    }
    for (int i = 0; i < 11; i++) {
        if (!isdigit(cpfEntrada[i])) {
            printf("CPF deve conter apenas números!\n");
            return 0;
        }
    }
    rewind(arqPessoa);
    struct cliente temp;

    while (fread(&temp, sizeof(struct cliente), 1, arqPessoa) == 1) {
        if (strcmp(temp.cpf, cpfEntrada) == 0) {
            printf("CPF já cadastrado!\n");
            return 0;
        }
    }

    return 1;
}
int conferirEmail(char *email) {

    if (strchr(email, '@') == NULL)
        return 0;

    const char *dom1 = "@gmail.com";
    const char *dom2 = "@hotmail.com";
    const char *dom3 = "@outlook.com";

    if (strstr(email, dom1) || strstr(email, dom2) || strstr(email, dom3))
        return 1;

    return 0;
}
void cadastro(struct cliente *P, int *qtd, FILE *arqPessoa) {
    int valido = 0;

    getchar(); 

    printf("Nome do cliente: ");
    fgets(P->nome, 50, stdin);
    P->nome[strcspn(P->nome, "\n")] = '\0';
    while (!valido) {
        printf("CPF (11 dígitos): ");
        fgets(P->cpf, 12, stdin);
        P->cpf[strcspn(P->cpf, "\n")] = '\0';

        valido = conferirCpf(P->cpf, arqPessoa);
    }
    printf("Contato (telefone): ");
    fgets(P->contato, 20, stdin);
    P->contato[strcspn(P->contato, "\n")] = '\0';
    valido = 0;
    while (!valido) {
        printf("Email: ");
        fgets(P->email, 50, stdin);
        P->email[strcspn(P->email, "\n")] = '\0';

        if (!conferirEmail(P->email)) {
            printf("Email inválido! Aceito apenas @gmail.com, @hotmail.com ou @outlook.com.\n");
        } else {
            valido = 1;
        }
    }

    printf("Quantidade de compras: ");
    scanf("%d", &P->compras);
    fseek(arqPessoa, 0, SEEK_END);
    fwrite(P, sizeof(struct cliente), 1, arqPessoa);

    (*qtd)++;
}

void listarClientes(FILE *arqPessoa) {
    struct cliente temp;

    rewind(arqPessoa);

    printf("\n=== CLIENTES CADASTRADOS ===\n");

    while (fread(&temp, sizeof(struct cliente), 1, arqPessoa) == 1) {
        printf("\n------------------------\n");
        printf("Nome: %s\n", temp.nome);
        printf("CPF: %s\n", temp.cpf);
        printf("Contato: %s\n", temp.contato);
        printf("Email: %s\n", temp.email);
        printf("Compras: %d\n", temp.compras);
    }
}

void buscarClientePorCPF(FILE *arqPessoa) {
    char cpfBusca[12];
    struct cliente temp;
    int encontrado = 0;

    printf("\nDigite o CPF para buscar: ");
    getchar();
    fgets(cpfBusca, 12, stdin);
    cpfBusca[strcspn(cpfBusca, "\n")] = '\0';

    if (strlen(cpfBusca) != 11) {
        printf("CPF deve ter 11 dígitos!\n");
        return;
    }

    rewind(arqPessoa);

    while (fread(&temp, sizeof(struct cliente), 1, arqPessoa) == 1) {
        if (strcmp(temp.cpf, cpfBusca) == 0) {
            encontrado = 1;

            printf("\n=== CLIENTE ENCONTRADO ===\n");
            printf("Nome: %s\n", temp.nome);
            printf("CPF: %s\n", temp.cpf);
            printf("Contato: %s\n", temp.contato);
            printf("Email: %s\n", temp.email);
            printf("Compras: %d\n", temp.compras);
            break;
        }
    }

    if (!encontrado)
        printf("Nenhum cliente encontrado com esse CPF.\n");
}


int main() {

    struct cliente pessoa;
    int escolha = -1, contador = 0;
    char yn;

    FILE *arqPessoa = fopen("clientes.dat", "ab+");

    if (arqPessoa == NULL) {
        printf("Erro ao abrir arquivo!\n");
        return 1;
    }

    while (escolha != 0) {

        printf("\n====== MENU ======\n");
        printf("1) Cadastrar cliente\n");
        printf("2) Listar clientes\n");
        printf("3) Buscar cliente por CPF\n");
        printf("0) Sair\n");
        printf("Escolha: ");
        scanf("%d", &escolha);

        switch (escolha) {

        case 1:
            yn = 'S';
            while (yn == 'S' || yn == 's') {
                cadastro(&pessoa, &contador, arqPessoa);

                printf("Cadastrar outro? (S/N): ");
                getchar();
                yn = getchar();
            }
            break;

        case 2:
            listarClientes(arqPessoa);
            break;

        case 3:
            buscarClientePorCPF(arqPessoa);
            break;

        case 0:
            printf("Saindo...\n");
            break;

        default:
            printf("Opção inválida!\n");
        }
    }

    fclose(arqPessoa);
    return 0;
}
