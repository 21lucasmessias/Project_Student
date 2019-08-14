#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <time.h>

typedef struct tipoAluno tipoAluno;
typedef struct tipoAproveitamento tipoAproveitamento;
typedef struct tipoDisciplina tipoDisciplina;
typedef struct tipoTurma tipoTurma;
typedef struct tipoProfessor tipoProfessor;

typedef struct tipoLoginProfessor tipoLoginProfessor;
struct tipoLoginProfessor{
	char login[80];
	char senha[80];
	tipoLoginProfessor *prox;
};

typedef struct tipoLoginAluno tipoLoginAluno;
struct tipoLoginAluno{
	char login[20];
	char senha[50];
	tipoLoginAluno *prox;
};

struct tipoAluno{
	char nome[80];
	int ra;
	tipoAluno *aluno;
	tipoAproveitamento *aproveitamento;
};

struct tipoAproveitamento{
	float notas[3];
	float frequencia;
	tipoTurma *turma;
	tipoAproveitamento *aproveitamento;
};

struct tipoDisciplina{
	int codigo;
	int cargahoraria;
	char nome[80];
	tipoDisciplina *disciplina;
};

struct tipoTurma{
	int codigo;
	int ano;
	int semestre;
	int quantiaAlunos;
	char situacao;
	tipoProfessor *professor;
	tipoDisciplina *disciplina;
	tipoAluno *aluno;
	tipoTurma *turma;
};

struct tipoProfessor{
	char nome[80];
	int anoEntrada;
	int anoSaida;
	tipoTurma *turmas;
	tipoProfessor *professor;
};

//--------------------------------------------------------------------------------------------------------
int len(char *RA){
	int i = 1;
	while(RA[i] != '\0'){
		i++;
	}
	return i;
}

int converterRA(char *RA){
	int Ra = 0;
	int j = 1;
	int i;
	
	for(i = 1 ; i < len(RA) ; i++){
		j = j*10;
	}

	for(i = 0 ; i < len(RA) ; i++){
		Ra += (RA[i] - 48) * j;
		j = j/10;
	}
	return Ra;
}

int anoAtual(){
    time_t mytime;
    mytime = time(NULL);
    struct tm tm = *localtime(&mytime);
    return tm.tm_year + 1900;
}

void cifraCesar(char *nome){
	int i = 0;
	while(nome[i] != '\0'){
		*(nome+i) = (*(nome+i)+3);
		i++;
	}
}
//--------------------------------------------------------------------------------------------------------
tipoAluno *buscaAluno(tipoAluno *primA, int ra){
	if(!primA) return NULL;
	tipoAluno *aux = primA;
	do{
		if(aux->ra == ra) return aux;
		aux = aux->aluno;
	}while(aux);
	return NULL;
}

tipoProfessor *buscaProfessor(tipoProfessor *primP, char *nome){
	if(!primP) return NULL;
	tipoProfessor *aux = primP;
	do{
		if(!strcmp(aux->nome, nome)) return aux;
		aux = aux->professor;
	}while(aux);
	return NULL;
}

tipoDisciplina *buscaDisciplina(tipoDisciplina *primD, char *nome){
	if(!primD) return NULL;
	tipoDisciplina *aux = primD;
	do{
		if(!strcmp(aux->nome, nome)) return aux;
		aux = aux->disciplina;
	}while(aux);
	return NULL;
}

tipoTurma *buscaTurma(tipoTurma *primT, int codigo, int ano, int semestre){
	if(!primT) return NULL;
	tipoTurma *aux = primT;
	do{
		if(aux->codigo == codigo && aux->ano == ano && aux->semestre == semestre) return aux;
		aux = aux->turma;
	}while(aux);
	return NULL;
}
//--------------------------------------------------------------------------------------------------------
int verificarDuplicidadeAluno(tipoAluno *aluno, char *nome, int RA){
	do{
		if(!strcmp(aluno->nome, nome)){
			printf("Ja existe um aluno com esse nome.\n");
			return 1;
		}
		if(aluno->ra == RA){
			printf("Ja existe um aluno com esse RA.\n");
			return 1;
		}
		aluno = aluno->aluno;
	}while(aluno);
	return 0;
}

int verificarDuplicidadeProfessor(tipoProfessor *prof, char *nome){
	do{
		if(!strcmp(prof->nome, nome)){
			printf("Ja existe um professor com esse nome.\n");
			return 1;
		}
		prof = prof->professor;
	}while(prof);
	return 0;
}

int verificarDuplicidadeDisciplina(tipoDisciplina *primD, char *nome, int codigo){
	do{
		if(!strcmp(primD->nome, nome)){
			printf("Ja existe uma discplina com esse nome.\n");
			return 1;
		}
		if(primD->codigo == codigo){
			printf("Ja existe uma disciplina com esse codigo.\n");
			return 1;
		}
		primD = primD->disciplina;
	}while(primD);
	return 0;
}
//--------------------------------------------------------------------------------------------------------
int insereAluno(tipoAluno **primA, int ra, char *nome){
	assert(primA);

	tipoAluno *novoAluno = NULL;

	novoAluno = (tipoAluno *)malloc(sizeof(tipoAluno));
	if(!(novoAluno)){
		printf("Memoria insuficiente para alocar aluno %s!", nome);
		return 1;
	}

	strcpy(novoAluno->nome, nome);
	novoAluno->ra = ra;
	novoAluno->aluno = *primA;
	novoAluno->aproveitamento = NULL;

	*primA = novoAluno;

	return 0;
}

int insereProfessor(tipoProfessor **primP, char *nome, int anoentrada, int anosaida){
	assert(primP);

	tipoProfessor *novoProfessor = NULL;

	novoProfessor = (tipoProfessor *)malloc(sizeof(tipoProfessor));
	if(!(novoProfessor)){
		printf("Memoria insuficiente para alocar professor %s!\n", nome);
		return 1;
	}

	strcpy(novoProfessor->nome, nome);
	novoProfessor->anoEntrada = anoentrada;
	novoProfessor->anoSaida = anosaida;
	novoProfessor->turmas = NULL;
	novoProfessor->professor = *primP;

	*primP = novoProfessor;

	return 0;
}

int insereDisciplina(tipoDisciplina **pD, char *nome, int codigo, int cargahoraria){
	assert(pD);

	tipoDisciplina *novaDisciplina = NULL;

	novaDisciplina = (tipoDisciplina *)malloc(sizeof(tipoDisciplina));
	if(!novaDisciplina){
		printf("Memoria insuficiente para alocar disciplina %d!\n", codigo);
		return 1;
	}

	strcpy(novaDisciplina->nome, nome);
	novaDisciplina->codigo = codigo;
	novaDisciplina->cargahoraria = cargahoraria;
	novaDisciplina->disciplina = *pD;

	*pD = novaDisciplina;

	return 0;
}

int insereTurma(tipoTurma **pT, int codigo, int ano, int semestre, int quantiaAlunos, char situacao, tipoProfessor *prof, tipoDisciplina *disc, tipoAluno *alunos){
	assert(pT);

	tipoTurma *novaTurma = NULL;

	novaTurma = (tipoTurma *)malloc(sizeof(tipoTurma));
	if(!novaTurma){
		printf("Memoria insuficiente para alocar turma %d!\n", codigo);
		return 1;
	}

	novaTurma->codigo = codigo;
	novaTurma->ano = ano;
	novaTurma->semestre = semestre;
	novaTurma->situacao = situacao;
	novaTurma->professor = prof;
	novaTurma->disciplina = disc;
	novaTurma->quantiaAlunos = quantiaAlunos;
	novaTurma->aluno = alunos;
	novaTurma->turma = *pT;
	novaTurma->professor->turmas = novaTurma;

	*pT = novaTurma;

	return 0;
}
//--------------------------------------------------------------------------------------------------------
void adicionarAlunoTurma(tipoAluno *aluno, tipoTurma *turma){
	if(!turma || !aluno) return;

	tipoAluno *aux = NULL;
	aux = (tipoAluno *)malloc(sizeof(tipoAluno));
	if(!aux) return;

	tipoAproveitamento *aux_aproveitamento = NULL;
	aux_aproveitamento = (tipoAproveitamento *)malloc(sizeof(tipoAproveitamento));
	if(!aux_aproveitamento) return;

	aux_aproveitamento->notas[0] = 0;
	aux_aproveitamento->notas[1] = 0;
	aux_aproveitamento->notas[2] = 0;
	aux_aproveitamento->frequencia = 0;
	aux_aproveitamento->turma = turma;
	aux_aproveitamento->aproveitamento = aluno->aproveitamento;

	aluno->aproveitamento = aux_aproveitamento;

	*aux = *aluno;
	aux->aluno = turma->aluno;
	turma->aluno = aux;
	(turma->quantiaAlunos)++;
}

void learq(tipoAluno **pA, tipoProfessor **pP, tipoDisciplina **pD, tipoTurma **pT){
	assert(pA);
	assert(pP);
	assert(pT);
	assert(pD);
	FILE *f;
	int flag;

	//--------- leitura de arquivo de alunos e criação da lista -----------

	f = fopen("aluno.txt", "r");
	if(!f){
		printf("Erro ao abrir aluno.txt!");
		return;
	}

	char nome[80], praq[5];
	int RA_int;
	while(!(feof(f))){		
		fscanf(f, "%s %d %s %s", nome, &RA_int, praq, praq);
		insereAluno(pA, RA_int, nome);
	}


	fclose(f);

	//--------- leitura de arquivo de professor e criação da lista -----------
	f = fopen("professor.txt", "r");
	if(!f){
		printf("Erro ao abrir professor.txt!");
		return;
	}
	
	int anoentrada, anosaida;

	while(!(feof(f))){
		fscanf(f, "%s %d %d %s %s", nome, &anoentrada, &anosaida, praq, praq);
		insereProfessor(pP, nome, anoentrada, anosaida);
	}

	fclose(f);

	//--------- leitura de arquivo de disciplina e criação da lista -----------
	f = fopen("disciplina.txt", "r");
	if(!f){
		printf("Erro ao abrir arquivo!");
		return;
	}

	int codigo, cargahoraria;

	while(!(feof(f))){
		fscanf(f, "%d %d %s %s", &codigo, &cargahoraria, nome, praq);
		insereDisciplina(pD, nome, codigo, cargahoraria);
	}

	fclose(f);

	//--------- leitura de arquivo de turma e criação da lista -----------
	f=fopen("turma.txt", "r");
	if(!f){
		printf("Erro ao abrir arquivo!");
		return;
	}
	int ano, semestre;
	char situacao;
	char nomeProfessor[80];
	char nomeDisciplina[80];
	tipoProfessor *prof = NULL;
	tipoDisciplina * disc = NULL;
	char RA[10] = {"1"};
	tipoAluno *alunos = NULL;

	while(!(feof(f))){
		fscanf(f, "%d %d %d %c %s %s", &codigo, &ano, &semestre, &situacao, nomeProfessor, nomeDisciplina);
		prof = buscaProfessor(*pP, nomeProfessor);
		disc = buscaDisciplina(*pD, nomeDisciplina);
		if(prof && disc){
			flag = insereTurma(pT, codigo, ano, semestre, 0, situacao, prof, disc, NULL);
			if(!flag){
				while(strcmp(RA, "NULL")){
					adicionarAlunoTurma(buscaAluno(*pA, converterRA(RA)), *pT);
					fscanf(f, "%s", RA);
				}
				RA[0] = 1;
			}
		}else{
			while(strcmp(RA, "NULL")){
				fscanf(f, "%s", RA);
			}
			RA[0] = 1;
		}
	}

	fclose(f);
}

void printar(tipoAluno *pA, tipoProfessor *pP, tipoTurma *pT, tipoDisciplina *pD){
	//printa lista aluno
	printf("-------------Lista de alunos-------------\n");
	if(!pA){
		printf("A lista de alunos esta vazia!\n");
	}else{
		do{
			printf("NOME: %s\nRA: %d\n\n", pA->nome, pA->ra);
			pA = pA->aluno;
		}while(pA);
	}
		
	//printa lista professor
	printf("+-------------Lista de professor-------------+\n");
	if(!pP){
		printf("A lista de professores esta vazia!\n");
	}else{
		do{
			printf("NOME: %s\nANO ENTRADA: %d\nANO SAIDA: %d\n\n", pP->nome, pP->anoEntrada, pP->anoSaida);
			pP = pP->professor;
		}while(pP);
	}
	
	//printa lista turma
	printf("+-------------Lista de turma-------------+\n");
	if(!pT){
		printf("A lista de turmas esta vazia!\n");
	}else{
		do{
			printf("CODIGO: %d\nANO: %d\nSEMESTRE: %d\nSITUACAO: %c\nPROFESSOR: %s\nDISCIPLINA: %s\nALUNOS: ", pT->codigo, pT->ano, pT->semestre, pT->situacao, pT->professor->nome, pT->disciplina->nome);
			pA = pT->aluno;
			if(pA){
				do{
					printf("%s ", pA->nome);
					pA = pA->aluno;
				}while(pA);
			}
			printf("\n\n");
			pT=pT->turma;
		}while(pT);
	}
	
	//printa lista disciplina
	printf("+-------------Lista de disciplina-------------+\n");
	if(!pD){
		printf("A lista de disciplinas esta vazia!\n");
	}else{
		do{
			printf("CODIGO: %d\nCH: %d\nNOME: %s\n\n", pD->codigo, pD->cargahoraria, pD->nome);
			pD = pD->disciplina;
		}while(pD);
	}
	printf("+--------------------------------------------+\n");	
}
//--------------------------------------------------------------------------------------------------------
void alterarDadosAluno(tipoAluno *aluno){
	int op;
	printf("1. Alterar nome\n");
	printf("0. Sair\n");
	scanf("%d", &op);
	if(op == 1){
		printf("Novo nome\n");
		scanf("%s", aluno->nome);
	}
	else if(op == 2){
		return;
	}else{
		printf("Opcao invalida!\n");
		alterarDadosAluno(aluno);
	}
}

void _realizarMatriculaGerenciador(tipoAluno *aluno, tipoTurma *turma){
	if(!aluno || !turma) return;

	tipoAproveitamento *aux = NULL;
	aux = (tipoAproveitamento *)malloc(sizeof(tipoAproveitamento));
	if(!aux) return;

	tipoAluno *aux_aluno = NULL;
	aux_aluno = (tipoAluno *)malloc(sizeof(tipoAluno));
	if(!aux_aluno) return;

	aux->notas[0] = 0;
	aux->notas[1] = 0;
	aux->notas[2] = 0;
	aux->frequencia = 0;
	aux->turma = turma;
	aux->aproveitamento = aluno->aproveitamento;
	aluno->aproveitamento = aux;

	*aux_aluno = *aluno;
	aux_aluno->aluno = turma->aluno;
	turma->aluno = aux_aluno;
	(turma->quantiaAlunos)++;
	printf("Inscricao realizada com sucesso!\n");
}

void realizarMatricula(tipoAluno *aluno, tipoTurma *turma){
	if(!aluno || !turma) return;
	if(turma->situacao != 'P' || turma->quantiaAlunos >= 45){
		printf("Turma fechada para matriculas!\n");
		return;
	}

	tipoAproveitamento *aux = NULL;
	aux = (tipoAproveitamento *)malloc(sizeof(tipoAproveitamento));
	if(!aux) return;

	tipoAluno *aux_aluno = NULL;
	aux_aluno = (tipoAluno *)malloc(sizeof(tipoAluno));
	if(!aux_aluno) return;

	aux->notas[0] = 0;
	aux->notas[1] = 0;
	aux->notas[2] = 0;
	aux->frequencia = 0;
	aux->turma = turma;
	aux->aproveitamento = aluno->aproveitamento;
	aluno->aproveitamento = aux;

	*aux_aluno = *aluno;
	aux_aluno->aluno = turma->aluno;
	turma->aluno = aux_aluno;
	(turma->quantiaAlunos)++;
	printf("Inscricao realizada com sucesso!\n");
}

void cancelarMatricula(tipoAluno *aluno, tipoTurma *turma){
	if(!aluno || !turma) return;

	tipoAluno *aux_aluno = turma->aluno;
	tipoAluno *cancelar;

	if(aux_aluno->ra == aluno->ra){
		cancelar = aux_aluno;
		turma->aluno = aux_aluno->aluno;
		free(cancelar);
	}else{
		while(aux_aluno->aluno->ra != aluno->ra){
			if(aux_aluno->aluno)
				aux_aluno = aux_aluno->aluno;
			else
				return;
		}
		cancelar = aux_aluno->aluno;
		aux_aluno->aluno = cancelar->aluno;
		free(cancelar);
	}
	
	tipoAproveitamento *aux_ap = aluno->aproveitamento;
	tipoAproveitamento *cancelar_ap;
	if(aux_ap->turma->codigo == turma->codigo && aux_ap->turma->ano == turma->ano && aux_ap->turma->situacao == turma->situacao){
		cancelar_ap = aluno->aproveitamento;
		aluno->aproveitamento = aux_ap->aproveitamento;
		free(cancelar_ap);
	}else{
		while(aux_ap->aproveitamento->turma->codigo != turma->codigo || aux_ap->aproveitamento->turma->ano != turma->ano || aux_ap->aproveitamento->turma->situacao != turma->situacao){
			if(aux_ap->aproveitamento)
				aux_ap = aux_ap->aproveitamento;
			else
				return;
		}
		cancelar_ap = aux_ap->aproveitamento;
		aux_ap->aproveitamento = cancelar_ap->aproveitamento;
		free(cancelar_ap);
	}
}

void verHistoricoCompleto(tipoAluno *aluno){
	if(!aluno) return;
	printf("--------------Historico Completo--------------\n\nAluno(a): %s\n\n", aluno->nome);
	if(!aluno->aproveitamento) return;
	tipoAproveitamento *historico = aluno->aproveitamento;
	do{
		printf("--------------------------------------\n");
		printf("Materia - %s\n", historico->turma->disciplina->nome);
		printf("Carga horaria - %d\n", historico->turma->disciplina->cargahoraria);
		printf("Ano - %d Semestre - %d\n", historico->turma->ano, historico->turma->semestre);
		printf("Media - %.2f\n", (historico->notas[0] + historico->notas[1] + historico->notas[2])/3);
		printf("Frequencia - %.1f%%\n\n", historico->frequencia);
		printf("--------------------------------------\n");

		historico = historico->aproveitamento;
	}while(historico);
}

void verHistoricoSemestre(tipoAluno *aluno, int ano, int semestre){
	if(!aluno) return;
	printf("--------------Historico Ano %d Semestre %d--------------\n\nAluno(a): %s\n\n", ano, semestre, aluno->nome);
	if(!aluno->aproveitamento) return;
	tipoAproveitamento *historico = aluno->aproveitamento;
	do{
		if(historico->turma->ano == ano && historico->turma->semestre == semestre){
			printf("--------------------------------------\n");
			printf("Materia - %s\n", historico->turma->disciplina->nome);
			printf("Carga horaria - %d\n", historico->turma->disciplina->cargahoraria);
			printf("Ano - %d Semestre - %d\n", historico->turma->ano, historico->turma->semestre);
			printf("Media - %.2f\n", (historico->notas[0] + historico->notas[1] + historico->notas[2])/3);
			printf("Frequencia - %.1f%%\n\n", historico->frequencia);
			printf("--------------------------------------\n");
		}
		historico = historico->aproveitamento;
	}while(historico);
}
//--------------------------------------------------------------------------------------------------------
void inserirNotas(char *nomeProf, tipoTurma *turma){
	if(!turma) return;
	
	char nomeDi[80];
    tipoTurma *aux;
    aux = turma;
    tipoAluno *aux_aluno;
	
	printf("Informe o nome da disciplina: ");
	scanf("%s", nomeDi);

    while(strcmp(aux->disciplina->nome, nomeDi) || aux->situacao != 'A' || strcmp(aux->professor->nome, nomeProf)){
        aux = aux->turma;
        if(!aux){
        	printf("Turma nao encontrada.\n");
        	return;
        }
    }

    aux_aluno = aux->aluno;
    while(aux_aluno){
        printf("Informe a nota do aluno(a) %s: \n", aux_aluno->nome);
        do{
        	printf(" - 1 nota: ");
        	scanf("%f", &(aux_aluno->aproveitamento->notas[0]));
        	if(aux_aluno->aproveitamento->notas[0] < 0 || aux_aluno->aproveitamento->notas[0]>10){
        		printf("Nota invalida!\n");
			}
		}while(aux_aluno->aproveitamento->notas[0] < 0 || aux_aluno->aproveitamento->notas[0]>10);
       
        do{
        	printf(" - 2 nota: ");
        	scanf("%f", &(aux_aluno->aproveitamento->notas[1]));
        	if(aux_aluno->aproveitamento->notas[1] < 0 || aux_aluno->aproveitamento->notas[1]>10){
        		printf("Nota invalida!\n");
			}
		}while(aux_aluno->aproveitamento->notas[1] < 0 || aux_aluno->aproveitamento->notas[1]>10);
		
        do{
        	printf(" - 3 nota: ");
        	scanf("%f", &(aux_aluno->aproveitamento->notas[2]));
        	if(aux_aluno->aproveitamento->notas[2] < 0 || aux_aluno->aproveitamento->notas[2]>10){
        		printf("Nota invalida!\n");
			}
		}while(aux_aluno->aproveitamento->notas[2] < 0 || aux_aluno->aproveitamento->notas[2]>10);
		aux_aluno=aux_aluno->aluno;
    }
}

void inserirFrequencia(char *nomeProf, tipoTurma *turma){
	if(!turma) return;
	
	char nomeDi[80];
    tipoTurma *aux;
    aux = turma;
    tipoAluno *aux_aluno;
	
	printf("Informe o nome da disciplina: ");
	scanf("%s", nomeDi);
	
    while(strcmp(aux->disciplina->nome, nomeDi) || aux->situacao != 'A' || strcmp(aux->professor->nome, nomeProf)){
        aux = aux->turma;
        if(!aux){
        	printf("Turma nao encontrada. \n");
        	return;
        }
    }

    aux_aluno = aux->aluno;
    while(aux_aluno){
        printf("Informe a frequencia do aluno(a) %s: \n", aux_aluno->nome);
        do{
        	printf(" - frequencia: ");
        	scanf("%f", &(aux_aluno->aproveitamento->frequencia));
        	if(aux_aluno->aproveitamento->frequencia<0 || aux_aluno->aproveitamento->frequencia>100){
            	printf("Frequencia invalida!\n");
        	}
    	}while(aux_aluno->aproveitamento->frequencia<0 || aux_aluno->aproveitamento->frequencia>100);
        aux_aluno = aux_aluno->aluno;
    }
}

void alteraNota(char *nomeProf, tipoTurma *turma){
	if(!turma) return;

    tipoTurma *aux;
    aux = turma;
    tipoAluno *aux_aluno;
    char nomeDi[80], nomeA[80];
	    
    printf("Informe o nome da disciplina: \n");
	scanf("%s", nomeDi);

    while(strcmp(aux->disciplina->nome, nomeDi) || aux->situacao != 'A' || strcmp(aux->professor->nome, nomeProf)){
        aux = aux->turma;
        if(!aux){
        	printf("Turma nao encontrada. \n");
        	return;
        }
    }

    aux_aluno = aux->aluno;
    
	printf("Informe o nome do aluno(a) que deseja alterar as notas: \n");
    scanf("%s", nomeA);
    
    while(strcmp(aux_aluno->nome, nomeA)){
		aux_aluno=aux_aluno->aluno;
		if(!aux_aluno){
			printf("Aluno(a) nao encontrado.\n");
			return;
		}
	}
    
    do{
    	printf("Nota anterior: %f\n", aux_aluno->aproveitamento->notas[0]);
       	printf(" - Informe a nova nota [1]: ");
        scanf("%f", &(aux_aluno->aproveitamento->notas[0]));
       	if(aux_aluno->aproveitamento->notas[0] < 0 || aux_aluno->aproveitamento->notas[0]>10){
        	printf("Nota invalida!\n");
		}
	}while(aux_aluno->aproveitamento->notas[0] < 0 || aux_aluno->aproveitamento->notas[0]>10);
    
    do{
       	printf("Nota anterior: %f\n", aux_aluno->aproveitamento->notas[1]);
       	printf(" - Informe a nova nota [2]: ");
       	scanf("%f", &(aux_aluno->aproveitamento->notas[1]));
       	if(aux_aluno->aproveitamento->notas[1] < 0 || aux_aluno->aproveitamento->notas[1]>10){
       		printf("Nota invalida!\n");
		}
	}while(aux_aluno->aproveitamento->notas[1] < 0 || aux_aluno->aproveitamento->notas[1]>10);
		
    do{
       	printf("Nota anterior: %f\n", aux_aluno->aproveitamento->notas[2]);
       	printf(" - Informe a nova nota [3]: ");
       	scanf("%f", &(aux_aluno->aproveitamento->notas[2]));
        if(aux_aluno->aproveitamento->notas[2] < 0 || aux_aluno->aproveitamento->notas[2]>10){
        	printf("Nota invalida!\n");
		}
	}while(aux_aluno->aproveitamento->notas[2] < 0 || aux_aluno->aproveitamento->notas[2]>10);
}

void alteraFrequencia(char *nomeProf, tipoTurma *turma){
	if(!turma) return;

    tipoTurma *aux;
    aux = turma;
    tipoAluno *aux_aluno;
    char nomeDi[80], nomeA[80];
	    
    printf("Informe o nome da disciplina: \n");
	scanf("%s", nomeDi);

    while(strcmp(aux->disciplina->nome, nomeDi) || aux->situacao != 'A' || strcmp(aux->professor->nome, nomeProf)){
        aux = aux->turma;
        if(!aux){
        	printf("Turma nao encontrada. \n");
        	return;
        }
    }

    aux_aluno = aux->aluno;
    
	printf("Informe o nome do aluno(a) que deseja alterar a frequencia: \n");
    scanf("%s", nomeA);
    
    while(strcmp(aux_aluno->nome, nomeA)){
		aux_aluno=aux_aluno->aluno;
		if(!aux_aluno){
			printf("Aluno(a) nao encontrado.\n");
			return;
		}
	}
    
    do{
    	printf("Frequencia atual do aluno(a): %f\n", aux_aluno->aproveitamento->frequencia);
        printf(" - Nova frequencia: ");
        scanf("%f", &(aux_aluno->aproveitamento->frequencia));
        if(aux_aluno->aproveitamento->frequencia<0 || aux_aluno->aproveitamento->frequencia>100){
            printf("Frequencia invalida!\n");
        }
    }while(aux_aluno->aproveitamento->frequencia<0 || aux_aluno->aproveitamento->frequencia>100);
}

void desempenhoAluno(tipoAluno *aluno, tipoTurma *turma){
	int raaux, ano, semestre, i, flag=0;
	float media;
	tipoAluno *aux, *aux_aluno;
	tipoTurma *auxT;
	aux=aluno;
	auxT=turma;
	
	printf("Informe o RA do aluno: ");
	scanf("%d", &raaux);
	aux=buscaAluno(aux, raaux);
	if(aux==NULL){
		printf("Aluno(a) nao encontrado.\n");
		return;
	}
	
	printf("Informe o ano que deseja pesquisar: ");
	scanf("%d", &ano);
	
	printf("Informe o semestre que deseja pesquisar: ");
	scanf("%d", &semestre);
	
	printf("+-----------------------------------------+\n");
	printf("Aluno: %s\n", aux->nome);
	printf("+-----------------------------------------+\n");
	while(auxT!=NULL){
		if(auxT->ano==ano && auxT->semestre==semestre && (auxT->situacao=='E' || auxT->situacao=='A')){
			aux_aluno=auxT->aluno;
			while(aux_aluno!=NULL){
				if(aux_aluno->ra==raaux){
					flag=1;
					printf("Disciplina: %s - Situacao (%c)\nAno: %d\n", auxT->disciplina->nome, auxT->situacao, auxT->ano);
					for(i=0; i<3; i++){
						media=media+aux_aluno->aproveitamento->notas[i];
					}
					media=(media/3);
					printf(" - Media: %f", media);
					if(media>=6){
						printf(" - Situacao: Aprovado!\n");
					}else{
						printf(" - Situacao: Reprovado!\n");
					}
					printf("+-----------------------------------------+\n");
				}
			aux_aluno=aux_aluno->aluno;
			}
		}
		auxT=auxT->turma;
	}
	if(flag==0){
		printf("Sem registros no ano de %d semestre %d.", ano, semestre);
	}
}

void desempenhoTurma(tipoTurma *turma){
	int ano, semestre, i, flag=0;
	char nomeDi[80];
	float media;
	tipoAluno *aux;
	tipoTurma *auxT;
	auxT=turma;
	
	printf("Informe o nome da disciplina: ");
	scanf("%s", nomeDi);
	printf("Informe o ano que deseja pesquisar: ");
	scanf("%d", &ano);
	printf("Informe o semestre que deseja pesquisar: ");
	scanf("%d", &semestre);
	
	printf("\n");
	
	while(auxT!=NULL){
		if(!strcmp(auxT->disciplina->nome, nomeDi) && auxT->ano==ano && auxT->semestre==semestre && (auxT->situacao=='E' || auxT->situacao=='A')){
			aux=auxT->aluno;
			while(aux!=NULL){
				flag=1;
				media=0;
				printf("+-----------------------------------------+\n");
				printf("Aluno: %s - Situacao da disciplina: (%c) \n", aux->nome, auxT->situacao);
			
				for(i=0; i<3; i++){
					media=media+aux->aproveitamento->notas[i];
				}
		
				media=(media/3.0);
				printf(" - Media: %f", media);
		
				if(media>=6){
					printf(" - Situacao: Aprovado!\n");
				}else{
					printf(" - Situacao: Reprovado!\n");
				}
			aux=aux->aluno;
			}
		}
	auxT=auxT->turma;
	}
	printf("+-----------------------------------------+\n");
	
	if(flag==0){
		printf("Sem registros sobre %s no ano de %d semestre %d.\n", nomeDi, ano, semestre);
	}
}

void verAprovados(tipoTurma *turma, char *nomeProf){
	tipoAluno *aux;
	tipoTurma *auxT;
	float media;
	int i, flag=0;
	auxT=turma;
	
	while(auxT!=NULL){
		flag=0;
		if(!strcmp(auxT->professor->nome, nomeProf) && (auxT->situacao =='E' || auxT->situacao =='A')){
			printf("\n+-----------------------------------------+\n");
			printf("Disciplina: %s - Situacao: (%c)\n", auxT->disciplina->nome, auxT->situacao);
			printf("Ano: %d - Semestre: %d\n", auxT->ano, auxT->semestre);
			
			aux=auxT->aluno;
			while(aux!=NULL){
				media=0;
				
				for(i=0; i<3; i++){
					media=media+aux->aproveitamento->notas[i];
				}
				media=(media/3.0);
		
				if(media>=6){
					flag=1;
					printf("+-----------------------------------------+\n");
					printf("Aluno: %s\nMedia: %f\n", aux->nome, media);
				}
			aux=aux->aluno;
			}
		if(flag==0){
			printf("0 registros de aprovacao na disciplina.\n");
		}
		}
		auxT=auxT->turma;
	}
	if(flag==0){
		printf("0 registros encontrados.\n");
	}
}

void verReprovados(tipoTurma *turma, char *nomeProf){
	tipoAluno *aux;
	tipoTurma *auxT;
	float media;
	int i, flag=0;
	auxT=turma;
	
	while(auxT!=NULL){
		flag=0;
		if(!strcmp(auxT->professor->nome, nomeProf) && (auxT->situacao =='E' || auxT->situacao =='A')){
			printf("+-----------------------------------------+\n");
			printf("Disciplina: %s - Situacao: (%c)\n", auxT->disciplina->nome, auxT->situacao);
			printf("Ano: %d - Semestre: %d\n", auxT->ano, auxT->semestre);
			printf("+-----------------------------------------+\n");
			
			aux=auxT->aluno;
			while(aux!=NULL){
				media=0;
				
				for(i=0; i<3; i++){
					media=media+aux->aproveitamento->notas[i];
				}
				media=(media/3.0);
		
				if(media<6){
					flag=1;
					printf("Aluno: %s\nMedia: %f\n\n", aux->nome, media);
				}
			aux=aux->aluno;
			}
		if(flag==0){
			printf("0 registros de reprovacao na disciplina.\n");
		}
		}
		auxT=auxT->turma;
	}
	if(flag==0){
		printf("0 registros encontrados.\n");
	}
}

void relatorioProf(tipoTurma *turma, char *nomeProf){
	tipoTurma *auxT;
	int flag=0;
	auxT=turma;
	
	while(auxT!=NULL){
		if(!strcmp(auxT->professor->nome, nomeProf)){
			flag=1;
			
			printf("+-----------------------------------------+\n");
			printf("Disciplina: %s - Situacao: (%c)\n", auxT->disciplina->nome, auxT->situacao);
			printf("Ano: %d - Semestre: %d\n", auxT->ano, auxT->semestre);
		}
		auxT=auxT->turma;
	}
	
	if(flag==0){
		printf("O historico esta vazio.\n");
	}
}

void encerrarTurmas(tipoTurma *turma, char *nomeProf){
	tipoTurma *auxT;
	int flag=0;
	auxT=turma;
	
	while(auxT!=NULL){
		if(!strcmp(auxT->professor->nome, nomeProf)){
			flag=1;
			auxT->situacao='E';
			
			printf("+-----------------------------------------+\n");
			printf("Disciplina: %s - Situacao: (%c)\n", auxT->disciplina->nome, auxT->situacao);
			printf("Ano: %d - Semestre: %d\n", auxT->ano, auxT->semestre);
		}
		auxT=auxT->turma;
	}
	
	if(flag==0){
		printf("Nenhuma turma encontrada.\n");
	}
}
//--------------------------------------------------------------------------------------------------------
void excluirDisciplina(tipoTurma *turma, tipoDisciplina **disciplina){
	char nomeDi[80];
	int flag=0;
	tipoTurma *aux;
	tipoDisciplina *auxD, *auxD1;
	aux=turma;
	auxD=*disciplina;
	auxD1=auxD->disciplina;
	
	if(*disciplina==NULL){
		printf("Nao existem disciplinas registradas.");
		return;
	}else{
		printf("Informe o nome da disciplina: ");
		scanf("%s", nomeDi);
	}
	
	while(aux!=NULL){
		if(!strcmp(aux->disciplina->nome, nomeDi)){
			printf("Impossivel excluir!\nExistem dados relacionados a disciplina selecionada.\n");
			return;
		}else{
			flag=1;
		}
		aux=aux->turma;
	}
	
	if(flag==1){
		if(auxD1==NULL && !strcmp(auxD->nome, nomeDi)){
			free(auxD);
			*disciplina=NULL;
			printf("Disciplina excluida!\n");
		}else if(auxD1!=NULL && !strcmp(auxD->nome, nomeDi)){
			*disciplina=auxD1;
			free(auxD);
			printf("Disciplina excluida!\n");
		}else{
			while(auxD1!=NULL){
				if(!strcmp(auxD1->nome, nomeDi)){
					auxD->disciplina=auxD1->disciplina;
					free(auxD1);
					printf("Disciplina excluida!\n");
					return;
				}
				flag=0;
				auxD=auxD->disciplina;
				auxD1=auxD1->disciplina;
			}
			if(flag==0){
				printf("Disciplina nao encontrada!\n");
				return;
			}
		}
	}
}

void excluirTurma(tipoTurma **turma){
	char nomeDi[80];
	int flag=0, ano, semestre;
	tipoTurma *aux, *aux1;
	aux=*turma;
	aux1=aux->turma;
	
	if(*turma==NULL){
		printf("+-----------------------------------------+\n");
		printf("Nao existem turmas registradas.");
		return;
	}else{
		printf("+-----------------------------------------+\n");
		printf("Informe o nome da disciplina: ");
		scanf("%s", nomeDi);
		printf("Informe o ano: ");
		scanf("%d", &ano);
		printf("Informe o semestre: ");
		scanf("%d", &semestre);
		printf("+-----------------------------------------+\n");
	}
	
	while(aux!=NULL){
		if(!strcmp(aux->disciplina->nome, nomeDi) && aux->ano==ano && aux->semestre==semestre && aux->aluno!=NULL && aux->professor!=NULL){
			printf("Impossivel excluir!\nExistem dados relacionados a turma selecionada.\n");
			return;
		}else{
			flag=1;
		}
		aux=aux->turma;
	}
	
	aux=*turma;
	
	if(flag==1){
		if(aux1==NULL && !strcmp(aux->disciplina->nome, nomeDi)){
			free(aux);
			*turma=NULL;
			printf("Turma excluida!\n");
		}else if(aux1!=NULL && !strcmp(aux->disciplina->nome, nomeDi)){
			*turma=aux1;
			free(aux);
			printf("Turma excluida!\n");
		}else{
			while(aux1!=NULL){
				if(!strcmp(aux1->disciplina->nome, nomeDi)){
					aux->turma=aux1->turma;
					free(aux1);
					printf("Turma excluida!\n");
					return;
				}
				flag=0;
				aux=aux->turma;
				aux1=aux1->turma;
			}
			if(flag==0){
				printf("Turma nao encontrada!\n");
				return;
			}
		}
	}
}

void excluirProfessor(tipoProfessor **professor){
	assert(professor);

	char nomeProf[80], nome[80], praq[10], senha[80];
	int flag=0, anoentrada, anosaida;
	tipoProfessor *aux, *aux1;
	aux = *professor;
	aux1 = aux->professor;

	FILE *f = NULL, *w = NULL;
	tipoLoginProfessor *professores = NULL;
	tipoLoginProfessor *aux_login = NULL;
	
	if(!(*professor)){
		printf("+-----------------------------------------+\n");
		printf("Nao existem professores registradas.");
		return;
	}else{
		printf("+-----------------------------------------+\n");
		printf("Informe o nome do professor: ");
		scanf("%s", nomeProf);
		printf("+-----------------------------------------+\n");
	}
	
	while(aux){
		if(!strcmp(aux->nome, nomeProf) && aux->turmas){
			printf("Impossivel excluir!\nExistem dados relacionados ao professor selecionado.\n");
			return;
		}else{
			flag=1;
		}
		aux=aux->professor;
	}
	
	aux=*professor;
	
	if(flag==1){
		if(aux1==NULL && !strcmp(aux->nome, nomeProf)){
			free(aux);
			*professor=NULL;
			printf("Professor excluido!\n");
		}else if(aux1!=NULL && !strcmp(aux->nome, nomeProf)){
			*professor=aux1;
			free(aux);
			printf("Professor excluido!\n");
		}else{
			while(aux1){
				if(!strcmp(aux1->nome, nomeProf)){
					aux->professor=aux1->professor;
					free(aux1);
					printf("Professor excluido!\n");
					aux1 = NULL;
					flag = 1;
				}else{
					flag=0;
					aux=aux->professor;
					aux1=aux1->professor;
				}
			}
			if(!flag){
				printf("Professor nao encontrado!\n");
				return;
			}
		}
		f = fopen("loginsProfessores.txt", "r");
		if(!f){
			printf("Erro ao ler loginsProfessores\n");
			return;
		}

		fscanf(f, "%s", nome);
		if(feof(f)){
			fclose(f);
			return;
		}else{
			rewind(f);
			cifraCesar(nomeProf);
			while(!(feof(f))){
				fscanf(f, "%s %s", nome, senha);
				if(strcmp(nome, nomeProf)){
					aux_login = (tipoLoginProfessor *)malloc(sizeof(tipoLoginProfessor));
					if(!aux_login){
						printf("Erro ao alocar aux_login\n");
						return;
					}
					strcpy(aux_login->login, nome);
					strcpy(aux_login->senha, senha);
					aux_login->prox = professores;
					professores = aux_login;
				}
			}
		}
		fclose(f);

		w = fopen("loginsProfessores.txt", "w");
		if(!w){
			printf("Erro ao escrever loginsProfessores\n");
			return;
		}
		
		aux_login = professores;
		
		if(aux_login){
			do{
				fprintf(w, "%s %s\n", aux_login->login, aux_login->senha);
				aux_login = aux_login->prox;
			}while(aux_login);
		}

		fclose(w);
	}
}

void excluirAluno(tipoAluno **aluno, tipoTurma *turma){
	int RA_int, flag=0;
	tipoAluno *aux, *aux1, *aux2;
	tipoTurma *auxT;
	aux=*aluno;
	aux1=aux->aluno;
	auxT=turma;

	FILE *f = NULL, *w = NULL;
	char raAluno[20], RA[20], senha[50];
	tipoLoginAluno *alunos = NULL;
	tipoLoginAluno *aux_Login = NULL;
	
	if(*aluno==NULL){
		printf("+-----------------------------------------+\n");
		printf("Nao existem alunos registrados.");
		return;
	}else{
		printf("+-----------------------------------------+\n");
		printf("Informe o RA do aluno: ");
		scanf("%s", raAluno);
		RA_int = converterRA(raAluno);
		cifraCesar(raAluno);
		printf("+-----------------------------------------+\n");
	}
	
	while(auxT!=NULL){
		aux2=auxT->aluno;
		while(aux2!=NULL){
			if(aux2->ra==RA_int){
				printf("Impossivel excluir!\nExistem dados relacionados ao aluno selecionado.\n");
				return;
			}else{
				flag=1;
			}
			aux2=aux2->aluno;
		}
		auxT=auxT->turma;
	}
	
	aux=*aluno;
	
	if(flag==1){
		if(aux1==NULL && aux->ra==RA_int){
			free(aux);
			*aluno=NULL;
			printf("Aluno excluido!\n");
		}
		else if(aux1!=NULL && aux->ra==RA_int){
			*aluno=aux1;
			free(aux);
			printf("Aluno excluido!\n");
		}else{
			while(aux1!=NULL){
				if(aux1->ra==RA_int){
					aux->aluno=aux1->aluno;
					free(aux1);
					printf("Aluno excluido!\n");
					flag = 1;
					aux1 = NULL;
				}else{
					flag=0;
					aux=aux->aluno;
					aux1=aux1->aluno;
				}
			}
			if(!flag){
				printf("Aluno nao encontrado!\n");
				return;
			}
		}

		f = fopen("loginsAlunos.txt", "r");
		if(!f){
			printf("Erro ao ler loginsAlunos\n");
			return;
		}

		fscanf(f, "%s", RA);
		if(feof(f)){
			fclose(f);
			return;
		}else{
			rewind(f);
			while(!(feof(f))){
				fscanf(f, "%s %s", RA, senha);
				if(strcmp(RA, raAluno)){
					aux_Login = (tipoLoginAluno *)malloc(sizeof(tipoLoginAluno));
					if(!aux_Login){
						printf("Erro ao alocar aux_Login\n");
						return;
					}
					strcpy(aux_Login->login, RA);
					strcpy(aux_Login->senha, senha);
					aux_Login->prox = alunos;
					alunos = aux_Login;
				}
			}
		}
		fclose(f);

		w = fopen("loginsAlunos.txt", "w");
		if(!w){
			printf("Erro ao escrever loginsAlunos\n");
			return;
		}

		aux_Login = alunos;
		
		if(aux_Login){
			do{
				fprintf(w, "%s %s\n", aux_Login->login, aux_Login->senha);
				aux_Login = aux_Login->prox;
			}while(aux_Login);
		}

		fclose(w);
	}
}
//--------------------------------------------------------------------------------------------------------
void menus(char *login, int RA_int, tipoAluno **primA, tipoProfessor **primP, tipoDisciplina **primD, tipoTurma **primT){
	char logComp[50], senhaComp[50];
	int opcao;
	int escolha=0;
	char nome[80], senha[50], RA[20];

	FILE *f = NULL;
	
	//menu para login de alunos
	f=fopen("loginsAlunos.txt", "r");
	if(!f){
		printf("Erro no arquivo Aluno!");
		return;
	}
	
	while(!(feof(f))){
		fscanf(f, "%s %s", logComp, senhaComp);
		strcat(logComp, senhaComp);
		if(!strcmp(logComp, login)){
			tipoAluno *alunoAtual = buscaAluno(*primA, RA_int);
			do{
				printf("+---------------------------------------------+\n");
				printf("|                  Selecione                  |\n");
				printf("+---------------------------------------------+\n");
				printf("| [1] - Alterar dados                         |\n");
				printf("| [2] - Realizar matricula em uma disciplina  |\n");
				printf("| [3] - Cancelar matricula em uma disciplina  |\n");
				printf("| [4] - Ver historico completo                |\n");
				printf("| [5] - Ver historico por semestre            |\n");
				printf("| [6] - Sair                                  |\n");
				printf("+---------------------------------------------+\n");
				scanf("%d", &opcao);
				if(opcao == 1){
					alterarDadosAluno(alunoAtual);
				}
				else if(opcao == 2){
					tipoTurma *aux_turma = NULL;
					int aux_cont, opcao_turma;
    				do{
    					printf("+---------------------------------------------+\n");
						printf("|            Selecione a Turma                |\n");
	    				printf("+---------------------------------------------+\n");
    					aux_turma = *primT;
    					aux_cont = 1;
	    				while(aux_turma){
	    					if(aux_turma->situacao == 'P'){
		    					printf("%d - %s\n", aux_cont, aux_turma->disciplina->nome);
	    						aux_cont++;
		    				}
	    					aux_turma = aux_turma->turma;
	    				}
	    				printf("0 - Cancelar.\n");
	    				printf("+---------------------------------------------+\n");
	    				scanf("%d", &opcao_turma);
	    			}while(opcao_turma < 0 || opcao_turma > (aux_cont-1));

	    			if(opcao_turma){
	    				aux_turma = *primT;

	    				while(aux_turma){
	    					if(aux_turma->situacao == 'P'){
	    						opcao_turma--;
	    						if(!opcao_turma) break;
	    					}
	    					aux_turma = aux_turma->turma;
	    				}
    					realizarMatricula(alunoAtual, aux_turma);
    				}
				}
				else if(opcao == 3){
					tipoAproveitamento *aux_ap = NULL;
					int aux_cont, opcao_turma;
    				do{
    					aux_ap = alunoAtual->aproveitamento;
    					aux_cont = 1;
    					printf("+---------------------------------------------+\n");
						printf("|            Selecione a Turma                |\n");
	    				printf("+---------------------------------------------+\n");
    			
	    				while(aux_ap){
	    					if((aux_ap->notas[0] + aux_ap->notas[1] + aux_ap->notas[2]) == 0){
		    					printf("%d - %s\n", aux_cont, aux_ap->turma->disciplina->nome);
	    						aux_cont++;
		    				}
	    					aux_ap = aux_ap->aproveitamento;
	    				}

	    				printf("0 - Cancelar.\n");
	    				printf("+---------------------------------------------+\n");
	    				scanf("%d", &opcao_turma);
	    			}while(opcao_turma < 0 || opcao_turma > (aux_cont-1));

	    			if(opcao_turma){
	    				aux_ap = alunoAtual->aproveitamento;

	    				while(aux_ap){
	    					if((aux_ap->notas[0] + aux_ap->notas[1] + aux_ap->notas[2]) == 0){
	    						opcao_turma--;
	    						if(!opcao_turma) break;
	    					}
	    					aux_ap = aux_ap->aproveitamento;
	    				}
    					cancelarMatricula(alunoAtual, aux_ap->turma);
    				}
				}
				else if(opcao == 4){
					verHistoricoCompleto(alunoAtual);
				}
				else if(opcao == 5){
					int ano, semestre;
					printf("Ano: ");
					scanf("%d", &ano);
					printf("Semestre: ");
					scanf("%d", &semestre);
					verHistoricoSemestre(alunoAtual, ano, semestre);
				}
				else if(opcao == 6){
					printf("Bye\n");
				}
				else{
					printf("Opcao invalida!\n");
				}
			}while(opcao!=6);
			return;
		}
	}
	
	fclose(f);
	
	//menu para login de professores
	f=fopen("loginsProfessores.txt", "r");
	if(!f){
		printf("Erro no arquivo Professor!");
		return;
	}
	
	while(!(feof(f))){
		fscanf(f, "%s %s", logComp, senhaComp);
		strcat(logComp, senhaComp);
		if(!strcmp(logComp, login)){
			do{
				printf("+------------------------------------------------------------------+\n");
				printf("|                             Selecione                            |\n");
				printf("+------------------------------------------------------------------+\n");
				printf("| [1] - Inserir notas                                              |\n");
				printf("| [2] - Alterar notas                                              |\n");
				printf("| [3] - Inserir frequencia                                         |\n");
				printf("| [4] - Alterar frequencia                                         |\n");
				printf("| [5] - Desempenho de um aluno por semestre                        |\n");
				printf("| [6] - Desempenho de uma turma por semestre                       |\n");
				printf("| [7] - Ver alunos aprovados em todas as disciplinas que lecionou  |\n");
				printf("| [8] - Ver alunos reprovados em todas as disciplinas que lecionou |\n");
				printf("| [9] - Gerar relatorio completo                                   |\n");
				printf("| [10] - Encerrar Turma                                            |\n");
				printf("| [0] - Sair                                                       |\n");
				printf("+------------------------------------------------------------------+\n");
				scanf("%d", &opcao);
				if(opcao == 1){
					inserirNotas(nome, *primT);
				}
				else if(opcao == 2){
					alteraNota(nome, *primT);
				}
				else if(opcao == 3){
					inserirFrequencia(nome, *primT);
				}
				else if(opcao == 4){
					alteraFrequencia(nome, *primT);
				}
				else if(opcao == 5){
					desempenhoAluno(*primA, *primT);
				}
				else if(opcao == 6){
					desempenhoTurma(*primT);
				}
				else if(opcao == 7){
					verAprovados(*primT, nome);
				}
				else if(opcao == 8){
					verReprovados(*primT, nome);
				}
				else if(opcao == 9){
					relatorioProf(*primT, nome);
				}
				else if(opcao == 10){
					encerrarTurmas(*primT, nome);
				}
				else if(opcao == 0){
					printf("Bye\n");
				}
				else{
					printf("Opcao invalida!\n");
				}
			}while(opcao!=0);
			return;
		}
	}
	
	fclose(f);
	
	//menu para login de gerenciadores
	f=fopen("loginsGerenciadores.txt", "r");
	if(!f){
		printf("Erro no arquivo Gerenciador!");
		return;
	}
	
	while(!(feof(f))){
		fscanf(f, "%s %s", logComp, senhaComp);
		strcat(logComp, senhaComp);
		if(!strcmp(logComp, login)){
			fclose(f);
			do{
				printf("+------------------------------------------------------------------+\n");
				printf("|                             Selecione                            |\n");
				printf("+------------------------------------------------------------------+\n");
				printf("| [1] - Cadastrar aluno                                            |\n");
				printf("| [2] - Cadastrar disciplina                                       |\n");
				printf("| [3] - Cadastrar turma                                            |\n");
				printf("| [4] - Cadastrar professor                                        |\n");
				printf("| [5] - excluir disciplina                                         |\n");
				printf("| [6] - excluir turma                                              |\n");
				printf("| [7] - excluir professor                                          |\n");
				printf("| [8] - excluir aluno                                              |\n");
				printf("| [9] - Sair                                                       |\n");
				printf("+------------------------------------------------------------------+\n");
				scanf("%d", &opcao);

				if(opcao == 1){
					int aux_op;
					printf("Registrando aluno\n");
					printf("Nome:\n>");
					scanf("%s", nome);
					printf("RA:\n>");
					scanf("%s", RA);
					if(verificarDuplicidadeAluno(*primA, nome, converterRA(RA))){
						printf("Aluno/RA ja existente no banco, Continuar?\n1-Sim!\n2-Nao!\n");
						scanf("%d", &aux_op);
						if(aux_op == 1){
							printf("Senha:\n>");
							scanf("%s", senha);
							printf("+--------------------------------------------+\n");
							printf("Cursos disponiveis:\n> Ciencia da computacao\n[1]Cadastrar aluno\n[2]Sair\n");
							scanf("%d", &escolha);
							
							if(escolha==1){
								if(insereAluno(primA, converterRA(RA), nome)) printf("Erro ao adicionar aluno.\n");
								else{
									f = fopen("loginsAlunos.txt", "a");
									if(!f){
										printf("Erro no arquivo Aluno!");
										return;
									}
									cifraCesar(RA);
									cifraCesar(senha);
									fprintf(f, "%s %s\n", RA, senha);
									fclose(f);
								}
							}
							else if(escolha==2) printf("--BYE--\n");
						}
					}
				}
				else if(opcao == 2){
					int codigo, cargahoraria;
					printf("Nome disciplina: ");
					scanf("%s", nome);
					printf("Codigo: ");
					scanf("%d", &codigo);
					printf("Carga horaria: ");
					scanf("%d", &cargahoraria);
					if(verificarDuplicidadeDisciplina(*primD, nome, codigo))
						insereDisciplina(primD, nome, codigo, cargahoraria);
				}
				else if(opcao == 3){
					int codigo, ano, semestre, qtdalunos;
					char situacao;
					char nomeD[80];

					printf("Codigo: ");
					scanf("%d", &codigo);

					printf("Nome disciplina: ");
					scanf("%s", nomeD);

					ano = anoAtual();

					printf("Nome professor: ");
					scanf("%s", nome);

					if(!buscaProfessor(*primP, nome)){
						printf("Professor nao encontrado\n");
					}
					else if(!buscaDisciplina(*primD, nomeD)){
						printf("Disciplina nao encontrada\n");
					}else{
						printf("Semestre: ");
    					scanf("%d", &semestre);
    					
    					insereTurma(primT, codigo, ano, semestre, 0, 'P', buscaProfessor(*primP, nome), buscaDisciplina(*primD, nomeD), NULL);
					}
				}
				else if(opcao == 4){
					f=fopen("loginsProfessores.txt", "a");
					if(!f){
						printf("Erro no arquivo Professor!");
						return;
					}
			        printf("Nome professor: ");
					scanf("%s", nome);

					if(verificarDuplicidadeProfessor(*primP, nome)){
			            return;
			        }

					printf("Senha:\n>");
					scanf("%s", senha);

					if(insereProfessor(primP, nome, anoAtual(), 0)) return;

					cifraCesar(nome);
					cifraCesar(senha);

					fprintf(f, "%s %s\n", nome, senha);
					fclose(f);
				}
				else if(opcao == 5){
					excluirDisciplina(*primT, primD);
				}
				else if(opcao == 6){
					excluirTurma(primT);
				}
				else if(opcao == 7){
					excluirProfessor(primP);
				}
				else if(opcao == 8){
					excluirAluno(primA, *primT);
				}
				else if(opcao == 9){
					printf("Bye\n");
				}
				else{
					printf("Opcao invalida!\n");
				}
			}while(opcao!=9);
			return;
		}
	}
	printf("Erro! Usuario Inexistente.\n");
}

void login(tipoAluno **primA, tipoProfessor **primP, tipoDisciplina **primD, tipoTurma **primT){
	int escolha=0, anoentrada, anosaida, RA_int;
	char nome[80], login[50], senha[50], RA[20];

	char login_padrao_aluno[30] = {"StudentBCC%20#19&*"};
	char login_padrao_professor[30] = {"ProfessorBCC%20#19&*"};
	char login_padrao_gerenciador[30] = {"GerenciadorBCC%20#19&*"};
	cifraCesar(login_padrao_aluno);
	cifraCesar(login_padrao_professor);
	cifraCesar(login_padrao_gerenciador);

	printf("Login(RA):\n>");
	scanf("%s", login);
	printf("Senha:\n>");
	scanf("%s", senha);
	RA_int = converterRA(login);
	strcpy(nome, login);
	cifraCesar(login);
	cifraCesar(senha);
	strcat(login, senha);

	FILE *f = NULL;
	
	if(!strcmp(login_padrao_aluno, login)){//condicao aluno
		f = fopen("loginsAlunos.txt", "a");
		if(!f){
			printf("Erro no arquivo Aluno!");
			return;
		}
		
		printf("+--------------------------------------------+\n");
		printf("Bem vindo aluno!\nPara registrar-se, informe os campos abaixo:\n");
		printf("Nome:\n>");
		scanf("%s", nome);
		printf("RA:\n>");
		scanf("%s", RA);
		if(verificarDuplicidadeAluno(*primA, nome, converterRA(RA))){
			printf("Contate um gerenciador para solucionar suas duvidas!\n");
			return;
		}
		printf("Senha:\n>");
		scanf("%s", senha);
		printf("+--------------------------------------------+\n");
		printf("Cursos disponiveis:\n> Ciencia da computacao\n[1]Cadastrar-se\n[2]Sair\n");
		scanf("%d", &escolha);
		
		if(escolha==1){
			if(insereAluno(primA, converterRA(RA), nome)) printf("Erro ao adicionar aluno.\n");
			else{
				cifraCesar(RA);
				cifraCesar(senha);
				fprintf(f, "%s %s\n", RA, senha);
			}
		}
		else if(escolha==2){
			printf("--BYE--\n");
			return;
		}
		else printf("Opcao invalida!\n");
		fclose(f);
		menus(strcat(RA, senha), RA_int, primA, primP, primD, primT);
	}
	else if(!strcmp(login_padrao_professor, login)){//condicao professor
		f=fopen("loginsProfessores.txt", "a");
		if(!f){
			printf("Erro no arquivo Professor!");
			return;
		}
		printf("+--------------------------------------------+\n");
		printf("Bem vindo professor(a)!\n");
		printf("Nome:\n>");
		scanf("%s", nome);
		if(verificarDuplicidadeProfessor(*primP, nome)){
            printf("Contate o gerenciador.\n");
            return;
        }
		printf("Informe a senha:\n>");
		scanf("%s", senha);
		printf("+--------------------------------------------+\n");
		if(insereProfessor(primP, nome, anoAtual(), 0)) return;

		cifraCesar(nome);
		cifraCesar(senha);

		fprintf(f, "%s %s\n", nome, senha);
		fclose(f);
		menus(strcat(nome, senha), RA_int, primA, primP, primD, primT);
	}
	else if(!strcmp(login_padrao_gerenciador, login)){//condicao gerenciador
		f=fopen("loginsGerenciadores.txt", "a");
		if(!f){
			printf("Erro no arquivo Gerenciador!");
			return;
		}
		
		printf("Bem vindo gerenciador!\n");
		printf("Informe seu login:\n>");
		scanf("%s", login);
		printf("Informe a senha:\n>");
		scanf("%s", senha);

		cifraCesar(login);
		cifraCesar(senha);

		fprintf(f, "%s %s\n", login, senha);
		fclose(f);
		menus(strcat(login, senha), RA_int, primA, primP, primD, primT);
	}else{
		menus(login, RA_int, primA, primP, primD, primT);
	}
}

void atualizarDados(tipoAluno *primA, tipoProfessor *primP, tipoDisciplina *primD, tipoTurma *primT){
	FILE *f = NULL;

	f = fopen("aluno.txt", "w");
	while(primA){
		fprintf(f, "%s %d NULL NULL", primA->nome, primA->ra);
		if(primA->aluno)
			fprintf(f, "\n");
		primA = primA->aluno;
	}
	fclose(f);

	f = fopen("professor.txt", "w");
	while(primP){
		fprintf(f, "%s %d %d NULL NULL", primP->nome, primP->anoEntrada, primP->anoSaida);
		if(primP->professor)
			fprintf(f, "\n");
		primP = primP->professor;
	}
	fclose(f);

	f = fopen("disciplina.txt", "w");
	while(primD){
		fprintf(f, "%d %d %s NULL", primD->codigo, primD->cargahoraria, primD->nome);
		if(primD->disciplina)
			fprintf(f, "\n");
		primD = primD->disciplina;
	}
	fclose(f);

	f = fopen("turma.txt", "w");
	while(primT){
		fprintf(f, "%d %d %d %c %s %s ", primT->codigo, primT->ano, primT->semestre, primT->situacao, primT->professor->nome, primT->disciplina->nome);
		primA = primT->aluno;
		while(primA){
			fprintf(f, "%d ", primA->ra);
			primA = primA->aluno;
		}
		fprintf(f, "NULL");
		if(primT->turma)
			fprintf(f, "\n");
		primT = primT->turma;
	}
	fclose(f);
}

void main(){
	tipoAluno *primA = NULL;
	tipoProfessor *primP = NULL;
	tipoDisciplina *primD = NULL;
	tipoTurma *primT = NULL;

	learq(&primA, &primP, &primD, &primT);

	login(&primA, &primP, &primD, &primT);

	atualizarDados(primA, primP, primD, primT);
}
