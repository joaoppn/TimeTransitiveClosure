#include <bits/stdc++.h>

using namespace std;

// Classe para representar um contato temporal entre nós no grafo
class Rtuple {
public:
    int u, v;
    int tMinus, tPlus;
    int w = -1;

    // Construtor para contato sem peso
    Rtuple(int u, int v, int tM, int tP) : u(u), v(v), tMinus(tM), tPlus(tP) {}

    // Construtor para contato com peso
    Rtuple(int u, int v, int tM, int tP, int w) : u(u), v(v), tMinus(tM), tPlus(tP), w(w) {}

    // Função para verificar se este contato inclui outro
    bool includes(const Rtuple other) const {
        if (u == other.u && v == other.v) {
            if (other.tMinus <= tMinus && tPlus <= other.tPlus) {
                return true;
            } else {
                return false;
            }
        } else {
            return false;
        }
        // Alternativamente:
        // return this->u == other.u && this->v == other.v && this->tMinus == other.tMinus && this->tPlus == other.tPlus;
    }

    // Operador de comparação para ordenação (<)
    bool operator<(const Rtuple& other) const {
        return tPlus < other.tPlus;
    }

    // Operador de comparação para ordenação (<=)
    bool operator<=(const Rtuple& other) const {
        return tPlus <= other.tPlus;
    }

    // Verifica se o contato não está vazio
    bool not_empty() {
        if(tPlus != -1 or tMinus != -1) {
            //cout<< " naoVazio - "<<u<<v<<tMinus<<tPlus<< " ";
            return true;
        }
        //cout<<" vazio";
        return false;
    }
};

// Classe para representar um nó em um tempo específico
class nodeTime {
public:
    int node, time, sucessor;

    // Construtor para nó sem sucessor
    nodeTime(int node, int time) : node(node), time(time) {}

    // Construtor para nó com sucessor
    nodeTime(int node, int time, int sucessor) : node(node), time(time), sucessor(sucessor) {}

    // Operador de comparação para ordenação
    bool operator<(const nodeTime &nt) const {
        if (node != nt.node)
            return node < nt.node;
        else
            return time < nt.time;
    }

    // Operador de comparação para igualdade
    bool operator==(const nodeTime &nt) const {
        return (node == nt.node && time == nt.time);
    }
};

// Classe principal para representar o Grafo Temporal
class TemporalGraph {
public:
    int V;
    vector<vector<set<Rtuple>>> Treeset; // Matriz de árvores (conjuntos) de Rtuple
    int delta = 1;

    // Construtor para inicializar o grafo com um número de nós dado
    TemporalGraph(int x) : V(x) {
        Treeset.resize(x, vector<set<Rtuple>>(x));
    }

    //lida com entradas de u e v (int ou char)
    int get_node_index(const std::string& node_str) const {
            if (node_str.length() == 1 && isalpha(node_str[0])) {
                // Se for um único caractere alfabético (como 'a', 'b', etc.)
                return node_str[0] - 'a';
            } else {
                // Caso contrário, assume que é um número inteiro
                return std::stoi(node_str);
            }
        }

    // Retorna vizinhos de saída de um nó
    auto neighboursOut(int u) {
        set<int> nout;
        for (int j = 0; j < V; j++) {
            for (const Rtuple &r : Treeset[u][j]) {
                if (r.u == u)
                    nout.insert(r.v);
                else
                    nout.insert(r.u);
            }
        }
        return nout;
    }

    // Retorna vizinhos de entrada de um nó
    auto neighboursIn(int u) {
        set<int> nin;
        for (int j = 0; j < V; j++) {
            for (const Rtuple &r : Treeset[j][u]) {
                if (r.v == u) {
                    nin.insert(r.u);
                } else {
                    nin.insert(r.v);
                }
            }
        }
        return nin;
    }

    // Adiciona um novo contato temporal ao conjunto correspondente
void INSERT(int u, int v, int tMinus, int tPlus, int w) {
    std::set<Rtuple>& Tuv = Treeset[u][v];
    Rtuple newtuple(u, v, tMinus, tPlus);

    if (Tuv.empty()) {
        Tuv.insert(newtuple);
        return;
    }

    // FIXME: the following code is inefficient... it's just made to work
    std::set<Rtuple>::iterator it = Tuv.lower_bound(newtuple);

    if (it != Tuv.end() && *it <= newtuple) {
        return;
    }

    std::set<Rtuple> toBeRemoved;

    Rtuple inf = find_prev(u, v, tPlus);

    if (inf.tPlus == -1) {
        it = Tuv.begin();
        inf = *it;
    }

    for (; it != Tuv.end() && it->tPlus <= tPlus; ++it) {
        if (it->tMinus <= tPlus && it->includes(newtuple)) {
            toBeRemoved.insert(*it);
        }
    }

    for (const Rtuple& r : toBeRemoved) {
        Tuv.erase(r);
    }

    Tuv.insert(newtuple);
}


    // Encontra o contato temporal anterior em relação a um tempo especificado
    Rtuple find_prev(int u, int v, int t) {
        set<Rtuple> &Tuv = Treeset[u][v];
        Rtuple inf(u, v, -1, t);
        set<Rtuple>::iterator it = Tuv.lower_bound(inf);
        if (it == Tuv.begin()) {
            return Rtuple(u, v, -1, -1);
        } else {
            return *it;
        }
    };

    // Encontra o próximo contato temporal em relação a um tempo especificado
    Rtuple find_next(int u, int v, int t) {
        set<Rtuple> &Tuv = Treeset[u][v];
        Rtuple inf(u, v, -1, t);
        set<Rtuple>::iterator it = Tuv.upper_bound(inf);
        if (it == Tuv.end()) {
            return Rtuple(u, v, -1, -1);
        } else {
            return *it;
        }
    };

    // Verifica se é possível alcançar um nó de outro em um intervalo de tempo
    bool can_reach(string u_str, string v_str, int t1, int t2) {
        int u = get_node_index(u_str);
        int v = get_node_index(v_str);
        auto prev = find_next(u, v, t1);
        if (prev.not_empty() and prev.tPlus <= t2) {
            return true;
        } else {
            return false;
        }
    };

    // Adiciona um contato entre dois nós em um tempo específico
    void add_contact(string u_str, string v_str, int time) {
        int u = get_node_index(u_str);
        int v = get_node_index(v_str);
        INSERT(u, v, time, time + delta, v);

        set<nodeTime> D;

        for (int wMinus : neighboursIn(u)) {
            Rtuple prevRtuple = find_prev(wMinus, u, time);
            if (prevRtuple.not_empty()) {
                INSERT(wMinus, v, prevRtuple.tMinus, time + delta, prevRtuple.w);
                D.insert(nodeTime(wMinus, prevRtuple.tMinus, prevRtuple.w));
            }
        }

        for (int wPlus : neighboursOut(v)) {
            Rtuple next = find_next(v, wPlus, time + delta);

            if (next.not_empty()) {
                INSERT(u, wPlus, time, next.tPlus, v);

                for (nodeTime p : D) {
                    int wMinus = p.node;
                    int tMinus = p.time;
                    int successor = p.sucessor;
                    if (wMinus != wPlus) {
                        INSERT(wMinus, wPlus, tMinus, next.tPlus, successor);
                    }
                }
            }
        }
    }

    // Imprime o estado atual das árvores temporais
    void printTTC() {
        int a = 0, b = 0;
        for (auto x : Treeset) {
            for (auto y : x) {
                cout << "TreeSet[" << a << "][" << b << "] = ";
                for (auto k : y) {
                    cout << "(" << k.u << ", " << k.v << ", " << k.tMinus << ", " << k.tPlus << ") - ";
                }
                b++;
                cout << "\n";
            }
            a++;
            b = 0;
        }
    }
};

// Função principal

int main(int argc, char const *argv[]) {
    // Nome do arquivo a ser lido
    std::string nomeArquivo = "02.in";

    // Abre o arquivo
    std::ifstream arquivo(nomeArquivo);

    // Verifica se o arquivo foi aberto com sucesso
    if (!arquivo.is_open()) {
        std::cerr << "Erro ao abrir o arquivo." << std::endl;
        return 1; // Retorna 1 para indicar erro
    }

    // Lê o valor inicial do arquivo
    std::string linha;
    getline(arquivo, linha);

    // Converte a linha lida para um inteiro
    int numeroDeVertices = stoi(linha);

    // Cria uma instância de TemporalGraph com o valor lido do arquivo
    TemporalGraph teste(numeroDeVertices);

    // Lê e exibe cada linha restante do arquivo
    while (std::getline(arquivo, linha)) {
        std::istringstream ss(linha);
        std::string comando, u, v;
        ss >> comando;

        if (comando == "add") {
            // Comando de adicionar contato
            int t;
            ss >> u >> v >> t;
            teste.add_contact(u, v, t);
           // teste.add_contact(v,u,t);
        } else if (comando == "can_reach") {
            // Comando de verificar se é possível alcançar
            int t1, t2;
            ss >> u >> v >> t1 >> t2;
            if (teste.can_reach(u, v, t1, t2)) {
                std::cout << "true" << std::endl;
            } else {
                std::cout << "false" << std::endl;
            }
        } else {
            std::cout << "Comando inválido." << std::endl;
        }
    }

    // Imprime o estado final das árvores temporais
    teste.printTTC();

    // Fecha o arquivo
    arquivo.close();

    return 0; // Retorna 0 para indicar sucesso
}