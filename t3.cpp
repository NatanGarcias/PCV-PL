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

int main(){
    srand(time(0));  
    
    leitura();

//     rota[1].cidade = 4;
//     rota[2].cidade = 6;
//     rota[3].cidade = 8;
//     rota[4].cidade = 5;
//     rota[5].cidade = 1;
//     rota[6].cidade = 9;
//     rota[7].cidade = 3;
//     rota[8].cidade = 2;
//     rota[9].cidade = 7;

//    cout << fObj(rota) << endl;

    priority_queue<pair<long double, vector<Node>>>pq;

    do{
        auto f = fObj(rota);
        pq.push(make_pair(-f, rota));
    }while(next_permutation(rota.begin() + 1, rota.end()));
    
    while(!pq.empty()){
        cout << -pq.top().first << endl;
        for(auto i:pq.top().second) cout << i.cidade << " ";
        cout << endl << endl;
        pq.pop();
    }

    // AntColony();
    // RKGA();
    // GRASP+VND();
    // Exata();
}