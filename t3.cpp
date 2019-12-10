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
    int chegada;
    int saida;
    bool recebeuCarona;
};

struct Node{
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
        adj[u][v].desc = desc;
    }

    /*Le as distancias do vertices*/
    for(int i = 0; i < nV; i++)
        for(int j = 0; j < nV; j++)
            cin >> adj[i][j].dist;

    /*Le os passageiros*/
    for(int i = 0; i < nP; i++)
        cin >> vp[i].tarifaMax >> vp[i].chegada >> vp[i].saida;
}

bool tarifaMinima(vector<Node> &rota, vector<bool> &marcado, int indiceRota, int indicePassageiro){
    
    long double total = 0.0;
    
    for(int i = indiceRota; rota[i].cidade != vp[indicePassageiro].chegada; i++){
        
        int cidadeAtual = rota[i].cidade;
        int cidadeProxima = rota[i + 1].cidade;
        
        total += adj[cidadeAtual][cidadeProxima].dist;
        total -= adj[cidadeAtual][cidadeProxima].dist * (1.0 - adj[cidadeAtual][cidadeProxima].desc);
    }

    total /= C + 1;

    if(total > vp[indicePassageiro].tarifaMax){
        marcado[i] = true;
        return false;
    }
    
    return true;
}

long double calculaTarifa(vector<Node> &rota, int indicePassageiro, int indiceRota){
    
    long double total = 0.0;
    
    for(int i = indiceRota; rota[i].cidade != vp[indicePassageiro].chegada; i++){
        
        long double atual = 0.0;

        int cidadeAtual = rota[i].cidade;
        int cidadeProxima = rota[i + 1].cidade;
        
        long double atual = adj[cidadeAtual][cidadeProxima].dist;
        if(rota[i].numP == C)
            atual -= adj[cidadeAtual][cidadeProxima].dist * (1.0 - adj[cidadeAtual][cidadeProxima].desc);
    
        total += atual/(rota[i].numP + 1);
    }

    return total;
}

long double fObj(){
    
    /*Heuristica de Carregamento*/
    bool valido = false;
    vector<bool> marcado(nP, false);
    vector<pair<int, int>> passageiros;

    while(!valido){
        
        int numPassageiros = 0;
        vector<bool> visitado(nV, false);
        vector<int> desce(nP, 0);
        passageiros.clear();

        /*Alocando passageiros*/
        for(int i = 0; i < rota.size(); i++){

            int cidade = rota[i].cidade;

            /*Desaloca passageiros*/
            numPassageiros -= desce[cidade];
            desce[cidade] = 0;

            /*Aloca passageiros*/
            if(numPassageiros + 1 <= C && !visitado[vp[cidade].chegada] && !marcado[cidade] && tarifaMinima(rota, marcado, i, cidade)){
                numPassageiros++;
                rota[i].numP = numPassageiros;
                passageiros.push_back(make_pair(cidade, i));
                desce[vp[i].chegada]++;
            }
        }

        vector<long double>tarifas(nP, 0.0);

        long double maiorExcesso = 0.0;
        int maiorPassageiro = -1;

        for(int i = 0; i < passageiros.size(); i++){
            long double tarifaTotal = calculaTarifa(rota, passageiros[i].first, passageiro[i].second);
            if(maiorExcesso < tarifaTotal - vp[passageiros[i]].tarifaMax){
                maiorExcesso = tarifaTotal - vp[passageiros[i]].tarifaMax;
                maiorPassageiro = passageiros[i];
            }
        }

        if(maiorPassageiro != -1)
            marcado[maiorPassageiro] = true;
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
            atual -= adj[cidadeAtual][cidadeProxima].dist * (1.0 - adj[cidadeAtual][cidadeProxima].desc);
    
        total += atual/(rota[i].numP + 1);
    }

    return total;
}

int main(){
    srand(time(0));  
    leitura();
}