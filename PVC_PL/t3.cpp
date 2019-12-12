#include<bits/stdc++.h>
using namespace std;

int nV; //Numero de vertices
int nP; //Numero de passageiros
int C; //Capacidade do carro
int Q; //Numero de pedagios

struct Arco{
    Arco(): dist(0), desc(0), visitado(false){}
    int dist;
    long double desc;
    bool visitado;
};

struct Passageiro{
    Passageiro() : tarifaMax(0), chegada(-1), saida(-1), recebeuCarona(false){}
    long double tarifaMax;
    int saida;
    int chegada;
    bool recebeuCarona;
};

struct Node{
    bool operator<(const Node &other) const{ return cidade < other.cidade;}
    int cidade; 
    int numP;
};

vector<Node>rota; //Rota utilizada na solucao
vector<vector<Arco>> adj; //Matriz de adjacencia das cidades
vector<Passageiro> vp; //Vetor de passageiros

void tempoGasto(clock_t &t){
	printf ("It took me %d clicks (%f seconds).\n",t,((float)t)/CLOCKS_PER_SEC);
}

void leitura(){
    
    //Le os dados iniciais do problema
    cin >> nV >> nP >> C >> Q;
    
    adj.resize(nV);
    
    for(int i = 0; i < nV; i++) 
        adj[i].resize(nV);
    
    vp.resize(nP);
    
    rota.resize(nV);
    
    for(int i = 0; i < rota.size(); i++)
        rota[i] = {i, 0};

    /*Le os pedagios*/
    for(int i = 0; i < Q; i++){
        long double desc;
        int u, v;
        cin >> desc >> u >> v;
        adj[u][v].desc = adj[v][u].desc = desc;
    }

    /*Le as distancias do vertices*/
    for(int i = 0; i < nV; i++)
        for(int j = 0; j < nV; j++)
            cin >> adj[i][j].dist;

    /*Le os passageiros*/
    for(int i = 0; i < nP; i++)
        cin >> vp[i].tarifaMax >> vp[i].saida >> vp[i].chegada;
}

bool tarifaMinima(vector<Node> &rota, vector<bool> &marcado, int indiceRota, int indicePassageiro){
    
    long double total = 0.0;
    
    for(int i = indiceRota; rota[i].cidade != vp[indicePassageiro].chegada; i++){
        
        int cidadeAtual = rota[i].cidade;
        int cidadeProxima = rota[i + 1].cidade;
        
        total += adj[cidadeAtual][cidadeProxima].dist;
        total -= adj[cidadeAtual][cidadeProxima].dist * adj[cidadeAtual][cidadeProxima].desc;
    }

    total /= C + 1;

    if(total > vp[indicePassageiro].tarifaMax){
        marcado[indicePassageiro] = true;
        return false;
    }
    
    return true;
}

long double calculaTarifa(vector<Node> &rota, int indicePassageiro, int indiceRota){
    
    long double total = 0.0;
    
    for(int i = indiceRota; rota[i].cidade != vp[indicePassageiro].chegada; i++){
        
        int cidadeAtual = rota[i].cidade;
        int cidadeProxima = rota[i + 1].cidade;
        
        long double atual = adj[cidadeAtual][cidadeProxima].dist;
        if(rota[i].numP == C)
            atual -= adj[cidadeAtual][cidadeProxima].dist * adj[cidadeAtual][cidadeProxima].desc;
    
        total += atual/(rota[i].numP + 1);
    }

    return total;
}

long double fObj(vector<Node> &rota){
    
    /*Heuristica de Carregamento*/
    bool valido = false;
    vector<bool> marcado(nP, false);
    vector<pair<int, int>> passageiros;

    while(!valido){
        
        int numPassageiros = 0;
        
        vector<bool> visitado(nV, false);
        vector<int> desce(nP, 0);
        
        passageiros.clear();

        for(int i=0;i<rota.size();i++)
            rota[i].numP = 0;

        /*Alocando passageiros*/
        for(int i = 0; i < rota.size(); i++){

            int cidade = rota[i].cidade;

            /*Desaloca passageiros*/
            numPassageiros -= desce[cidade];
        
            /*Aloca passageiros, obs.: indice do passageiro = indice da cidade*/
            if(numPassageiros + 1 < C && !visitado[vp[cidade].chegada] && !marcado[cidade] && tarifaMinima(rota, marcado, i, cidade)){
                numPassageiros++;
                passageiros.push_back(make_pair(cidade, i));
                desce[vp[cidade].chegada]++;
            }

            rota[i].numP = numPassageiros;
            visitado[cidade] = true;
        }

        vector<long double>tarifas(nP, 0.0);

        long double maiorExcesso = 0.0;
        int maiorPassageiro = -1;

        for(int i = 0; i < passageiros.size(); i++){
            long double tarifaTotal = calculaTarifa(rota, passageiros[i].first, passageiros[i].second);
            //cerr << "T: " << passageiros[i].first << " " << tarifaTotal << endl;
            if(maiorExcesso < tarifaTotal - vp[passageiros[i].first].tarifaMax){
                maiorExcesso = tarifaTotal - vp[passageiros[i].first].tarifaMax;
                maiorPassageiro = passageiros[i].first;
            }
        }

        // for(int i=0;i<passageiros.size();i++){
        //     cout << passageiros[i].first << " ";
        // }cout << endl;

        if(maiorPassageiro != -1){
            //cout << maiorPassageiro << endl;
            marcado[maiorPassageiro] = true;
        }
        else
            valido = true;
    }

    long double total = 0.0;

    /*Calculo da Funcao Objetivo*/
    for(int i = 0; i < rota.size() - 1; i++){
        int cidadeAtual = rota[i].cidade;
        int cidadeProxima = rota[i + 1].cidade;
        
        long double atual = adj[cidadeAtual][cidadeProxima].dist;
        if(rota[i].numP == C)
            atual -= adj[cidadeAtual][cidadeProxima].dist * adj[cidadeAtual][cidadeProxima].desc;
    
        total += atual/(rota[i].numP + 1);

        //cout << cidadeAtual << " " << cidadeProxima << " " << adj[cidadeAtual][cidadeProxima].dist << " " << adj[cidadeAtual][cidadeProxima].dist/(rota[i].numP + 1) << " " << total << endl;

    }

    // for(int i=0;i<rota.size();i++){
    //     cout << rota[i].cidade << " ";
    // }cout << endl;

    // for(int i=0;i<rota.size();i++){
    //     cout << rota[i].numP << " ";
    // }cout << endl;

    // for(int i=0;i<passageiros.size();i++){
    //     cout << passageiros[i].first << " ";
    // }cout << endl;

    return total + adj[0][rota.back().cidade].dist; //Somando o custo de volta para a cidade inicial
}

bool pertuba(vector<Node> &rota){

	long double bestFObj = fObj(rota);
	bool melhor = false;
	
	vector<Node> rotaG;

	//Troco o vertice i com o vertice i+1
	//Se acho quma solucao melhor retorno esse solucao
	for(int i=1;i<nV-1;i++){
		swap(rota[i],rota[i+1]);

		long double atualFObj = fObj(rota);
		if(atualFObj < bestFObj){
			rotaG = rota;
			bestFObj = atualFObj;
			melhor = true;
		}

		swap(rota[i],rota[i+1]);		
	}

	if(melhor){
		rota = rotaG;
		return true;
	}

	return false;
}

void BL_GRASP(vector<Node> &rota){
	//Enquanto acho uma solução melhor 
	while(pertuba(rota));
}

void GRASP(int nIteracoes, int tlC){
	//Minimiza a distancia entre os vertices 

	//Calcular o tempo em milisegundos
	clock_t t;
  	t = clock();
 	
 	//Guarda a melhor rota até então encontrada
 	vector<Node> rotaG;

	long double bestFObj = std::numeric_limits<long double>::max();

	while(nIteracoes--){

		vector<Node> rota;
		vector<int> visited(nV,false);

		rota.push_back({0,0});
		visited[0] = true;

		for(int k=0;k<nV-1;k++){ //Enquanto não tenho todas as cidades na rota
			
			int bestV = -1;
			int bestD;

			int j = rota[rota.size()-1].cidade;
			
			priority_queue<pair<int,int>> lC; //lista Candidata
		
			for(int i=0;i<nP;i++){
				// Adiciono quem não foi visitado ainda a uma lista candidata que é feita por uma priority_queue
				// Salvo os valores negativos para pegar os que tem a menor distância	
				if(!visited[i]){ 
						bestD = adj[i][j].dist - (1.0 * adj[i][j].desc);
						lC.push({-bestD,i});
				}
			}

			//Defino o tamanho da lista candidata sendo o minimo entre um valor pré-definido para ela e a quantidade
			// de vertices que ainda não foram visitados
			int lC_tam = lC.size();
			int tamanhoListaCandidata = min(tlC,lC_tam);

			//Escolho o enesimo melhor vertice para ir tal que n é menor que o tamanho da lista candidata
			int random = rand()% tamanhoListaCandidata;

			while(random--){
				lC.pop();
			}

			bestV = lC.top().second; 

			//Adiciono o vertice escolhido na rota
			rota.push_back({bestV,0});
			visited[bestV] = true;
		}

		//Faço uma busca local na solução
		BL_GRASP(rota);
		
		//Verifico se encontrei uma solucao melhor que as que eu já vi antes
		long double atualFObj = fObj(rota);
			
		if(atualFObj < bestFObj){
			rotaG = rota;
			bestFObj = atualFObj; 
		}
	}

	//Imprimo a melhor solucao
	cout << "Melhor valor da funcao Objetivo: " << bestFObj << endl;
	for(auto i : rotaG) cout << i.cidade << " ";
	cout << endl;
	
	//Imprimo o tempo
	t = clock() - t;
	tempoGasto(t);
}

/*Gera um numero real aleatorio entre min e max (inclusos)*/
long double fRand(long double min, long double max){
    long double f = (long double)rand()/RAND_MAX;
    return min + f * (max - min);
}

/*Gera um novo individuo e salva suas chaves no vetor ind*/
void gerarIndividuo(vector<long double> &ind){
    for(int i = 0; i < ind.size(); i++)
        ind[i] = fRand(0, 1);
}

/*Decodifica chaves em solucoes com base no seguinte algoritmo:
a i-ésima cidade corresponde a posicao i - 1 do vetor de chaves,
que será ordenado em seguida, ditando assim a ordem das cidades
também*/
long double decoder(vector<long double> &ind){
    vector<pair<long double, int>> cidades(ind.size());
    
    for(int i = 0; i < cidades.size(); i++)
        cidades[i] = make_pair(ind[i], i + 1);

    sort(cidades.begin(), cidades.end());

    /*Construção da rota*/
    rota[0] = {0, 0}; //A rota sempre comeca do 0
    for(int i = 0; i < cidades.size(); i++)
        rota[i + 1] = {cidades[i].second, 0};

    return fObj(rota); //Retorna o resultado da função objetivo
}

void crossover(vector<long double> &paiE, vector<long double> &paiN, vector<long double> &filho){
    for(int i = 0; i < filho.size(); i++){
        int op = rand() % 100; //Opcao de recombinacao do gene
        
        if(op < 75)
            filho[i] = paiE[i];
        else
            filho[i] = paiN[i];
    }
}

void BRKGA(int popTam, long double pElite, long double pMut, int numIter, bool elitista = false){
    
	//Calcular o tempo em milisegundos
	clock_t t;
  	t = clock();
    
    /*Populacao vetores com chaves aleatorias*/
    vector<vector<long double>>pop(popTam, vector<long double>(nV - 1));
    
    /*Populacao que ira substituir a antiga (esta fora do while para nao ficar 
    alocando e desalocando sem necessidade)*/
    vector<vector<long double>>popNova(popTam, vector<long double>(nV - 1));

    /*Guarda os indices do individuos por ordem de fitness*/
    vector<pair<long double, int>> indices(popTam);
    
    /*Melhor solucao encontrada*/
    vector<long double> melhorInd;
    long double melhorFitness = numeric_limits<long double>::infinity();   

    /*Inicializa a populacao*/
    for(int i = 0; i < popTam; i++)
        gerarIndividuo(pop[i]);

    int numElite = popTam * pElite;
    int numMutantes = popTam * pMut;
    int numNormais = popTam - (numElite + numMutantes);

    int semMelhora = 0;

    while(semMelhora < numIter){

        semMelhora++;

        /*Avalia os individuos e constroi o conjunto elite*/
        for(int i = 0; i < popTam; i++){
            long double fitness = decoder(pop[i]);
            
            indices[i] = make_pair(fitness, i);

            /*Atualiza o melhor individuo*/
            if(fitness < melhorFitness){
                melhorFitness = fitness;
                melhorInd = pop[i];
                semMelhora = 0; //Houve melhora na iteração
            }
        }

        sort(indices.begin(), indices.end());

        /*Faz o crossover das solucoes de maneira elitista ou nao,
        depende do parametro utilizado*/
        for(int i = 0; i < numNormais; i++){
            int paiE, paiN; //indice dos pais
            
            /*Escolhe pais quaisquer*/
            paiE = indices[rand() % numElite].second; //Pai elite
            paiN = indices[numElite + rand() % (popTam - numElite)].second; //Pai normal

            //Gera o filho
            crossover(pop[paiE], pop[paiN], popNova[i]);
        }

        /*Mantem a elite na populacao*/
        for(int i = numNormais; i < numNormais + numElite; i++)
            popNova[i] = pop[indices[i - numNormais].second];
        
        /*Acrescenta os mutantes*/
        for(int i = numNormais + numElite; i < popTam; i++)
            gerarIndividuo(popNova[i]);

        pop = popNova; //Sobrescreve a populacao antiga
        cerr << "Melhor da geracao: " << decoder(melhorInd) << endl;
    }

    cout << "Melhor Individuo: " << decoder(melhorInd) << endl;

    //Tempo
	t = clock() - t;
	tempoGasto(t);
}

int main(){
    srand(time(0));  
    
    leitura();

    BRKGA(5000, 0.2, 0.1, 100);
    GRASP_One(nV*100,3);
}