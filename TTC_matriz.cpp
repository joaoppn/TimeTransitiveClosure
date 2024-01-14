#include <bits/stdc++.h>

using namespace std;

// Classe para representar um contato temporal entre nós no grafo
class Rtuple
{
public:
    int u, v;
    int tMinus, tPlus;
    

    // Construtor para contato sem peso
    Rtuple(int u, int v, int tM, int tP) : u(u), v(v), tMinus(tM), tPlus(tP) {}

    // Operador de comparação para ordenação (<)
    bool operator<(const Rtuple &other) const
    {
        if (tMinus != other.tMinus)
        {
            return tMinus < other.tMinus;
        }
        else
        {
            return tPlus < other.tPlus;
        }
    };
    // Operador de comparação para ordenação (<=)
    bool operator<=(const Rtuple &other) const
    {
        if (tMinus != other.tMinus)
        {
            return tMinus <= other.tMinus;
        }
        else
        {
            return tPlus <= other.tPlus;
        }
    };

    bool operator!=(const Rtuple &other) const
    {
        return tMinus != other.tMinus or tPlus != other.tPlus;
    }

    bool operator==(const Rtuple &other) const
    {
        return tMinus == other.tMinus or tPlus == other.tPlus;
    }

    bool contains(Rtuple other1, Rtuple other2)
    {
        if (!other1.not_empty())
        {
            return false; //(Tplus >= other2.Tminus);
        }
        else if (!other2.not_empty())
        {
            return (tMinus >= other1.tMinus);
        }

        return (tMinus >= other1.tMinus && tPlus <= other2.tPlus);
    }

    // Verifica se o contato não está vazio
    bool not_empty()
    {
        return tPlus != -1 or tMinus != -1;
    }
};

// Classe principal para representar o Grafo Temporal
class TemporalGraph
{
public:
    int V;
    vector<vector<set<Rtuple>>> Treeset; // Matriz de árvores (conjuntos) de Rtuple
    int delta = 1;

    // Construtor para inicializar o grafo com um número de nós dado
    TemporalGraph(int x) : V(x)
    {
        Treeset.resize(x, vector<set<Rtuple>>(x));
    }

    // lida com entradas de u e v (int ou char)
    int get_node_index(const std::string &node_str) const
    {
        if (node_str.length() == 1 && isalpha(node_str[0]))
        {
            // Se for um único caractere alfabético (como 'a', 'b', etc.)
            return node_str[0] - 'a';
        }
        else
        {
            // Caso contrário, assume que é um número inteiro
            return std::stoi(node_str);
        }
    }

    // Retorna vizinhos de saída de um nó [u][x]
    auto neighboursOut(int u)
    {
        set<int> nout;
        for (int j = 0; j < V; j++)
        {
            for (const Rtuple &r : Treeset[u][j])
            {
                if (r.u == u)
                    nout.insert(r.v);
                else
                    nout.insert(r.u);
            }
        }
        return nout;
    }

    // Retorna vizinhos de entrada de um nó [x][u]
    auto neighboursIn(int u)
    {
        set<int> nin;
        for (int j = 0; j < V; j++)
        {
            for (const Rtuple &r : Treeset[j][u])
            {
                if (r.v == u)
                {
                    nin.insert(r.u);
                }
                else
                {
                    nin.insert(r.v);
                }
            }
        }
        return nin;
    }

    // Adiciona um novo contato temporal ao conjunto correspondente
    void INSERT(int u, int v, int tMinus, int tPlus)
    {
        Rtuple newTuple(u, v, tMinus, tPlus);

        if (Treeset[u][v].empty())
        {
            set<Rtuple> newSet = {newTuple};
            Treeset[u][v] = newSet;
        }
        else
        {
            // Removendo redundâncias
            // Encontrando o limite inferior l1
            auto l1 = find_prev(u, v, tPlus);
            if (l1.tPlus < tPlus)
            {
                if (l1.tMinus >= tMinus)
                { // t1 t2
                    return;
                }
                else
                {
                    l1 = find_next(u, v, l1.tPlus);
                }
            }

            // Encontrando o limite superior l2
            auto l2 = find_next(u, v, tMinus);

            if (l2.not_empty() and l2.tMinus > tMinus)
            {
                l2 = find_prev(u, v, l2.tPlus);
            }

            // Verificando se l1 e l2 são valores aceitáveis

            set<Rtuple>::iterator it1, it2;

            if (!l1.not_empty())
            {
                it1 = Treeset[u][v].end();
            }
            else
            {
                it1 = Treeset[u][v].find(l1);
            }

            if (!l2.not_empty())
            {
                it2 = Treeset[u][v].end();
            }
            else
            {
                it2 = Treeset[u][v].find(l2);
            }

            // Conjunto temporário para armazenar elementos a serem removidos
            set<Rtuple> elementsToRemove;

            while (it1 != it2)
            {
                // Armazena o elemento em elementsToRemove em vez de removê-lo diretamente
                auto elem = *it1;
                if (elem.contains(l1, l2))
                {
                    elementsToRemove.insert(elem);
                }

                ++it1;
            }

            // Agora, remova os elementos do conjunto
            for (auto elem : elementsToRemove)
            {
                Treeset[u][v].erase(elem);
            }
            Treeset[u][v].insert(newTuple);
        }
    }

    // Encontra o contato temporal anterior em relação a um tempo especificado
    Rtuple find_prev(int u, int v, int t)
    {
        set<Rtuple> tuplas = Treeset[u][v];
        Rtuple latest(u, v, -1, -1);

        for (const auto &tupla : tuplas)
        {
            if (tupla.tPlus <= t)
            {
                latest = tupla;
            }
        }
        return latest;
    };

    // Encontra o próximo contato temporal em relação a um tempo especificado
    Rtuple find_next(int u, int v, int t)
    {
        set<Rtuple> &Tuv = Treeset[u][v];
        Rtuple inf(u, v, t, -1);
        set<Rtuple>::iterator it = Tuv.lower_bound(inf);
        if (it == Tuv.end())
        {
            return Rtuple(u, v, -1, -1);
        }
        else
        {
            return *it;
        }
    };

    // Verifica se é possível alcançar um nó de outro em um intervalo de tempo
    bool can_reach(string u_str, string v_str, int t1, int t2)
    {
        int u = get_node_index(u_str);
        int v = get_node_index(v_str);
        set<Rtuple> tuplas = Treeset[u][v];
        Rtuple next = find_next(u, v, t1);
        Rtuple prev = find_prev(u, v, t2);
        if ((next.not_empty() && next.tPlus <= t2) || (prev.not_empty() && prev.tPlus >= t1))
        {
            return true;
        }
        return false;
    };

    // Adiciona um contato entre dois nós em um tempo específico
    void add_contact(string u_str, string v_str, int time)
    {
        int u = get_node_index(u_str);
        int v = get_node_index(v_str);
        INSERT(u, v, time, time + delta);
        set<pair<int, int>> aux;
        set<int> neigh_a = neighboursIn(u);
        set<int> neigh_b = neighboursOut(v);
        // Neighbors in
        for (auto x : neigh_a)
        {
            Rtuple prev = find_prev(x, u, time);
            if (prev.not_empty())
            {
                INSERT(x, v, prev.tMinus, time + delta);
                aux.insert({x, prev.tMinus});
            }
        }
        // Neighbors out
        for (auto x : neigh_b)
        {
            Rtuple next = find_next(v, x, time + delta);
            if (next.tMinus > time + 1)
            {
                next = Rtuple(v, x, -1, -1);
            };
            if (next.not_empty())
            {
                INSERT(u, x, time, next.tPlus);
                for (auto tupla : aux)
                {
                    if (tupla.first != x)
                    {
                        INSERT(tupla.first, x, tupla.second, next.tPlus);
                    }
                }
            }
        }
    }

    // Imprime o estado atual das árvores temporais
    void printTTC()
    {
        int a = 0, b = 0;
        for (auto x : Treeset)
        {
            if (!x.empty())
            {
                for (auto y : x)
                {
                    if (!y.empty())
                    {
                        cout << "TreeSet[" << a << "][" << b << "] = ";
                        for (auto k : y)
                        {
                            cout << "(" << k.tMinus << ", " << k.tPlus << ") - ";
                        }

                        cout << "\n";
                    }
                    b++;
                }
                a++;
                b = 0;
            }
        }
    }
};

// Função principal
int main(int argc, char const *argv[])
{
    // Nome do arquivo a ser lido
    std::string nomeArquivo = "teste.in";

    // Abre o arquivo
    std::ifstream arquivo(nomeArquivo);

    // Verifica se o arquivo foi aberto com sucesso
    if (!arquivo.is_open())
    {
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
    while (std::getline(arquivo, linha))
    {
        std::istringstream ss(linha);
        std::string comando, u, v;
        ss >> comando;

        if (comando == "add")
        {
            // Comando de adicionar contato
            int t;
            ss >> u >> v >> t;
            teste.add_contact(u, v, t);
        }
        else if (comando == "can_reach")
        {
            // Comando de verificar se é possível alcançar
            int t1, t2;
            ss >> u >> v >> t1 >> t2;
            if (teste.can_reach(u, v, t1, t2))
            {
                std::cout << "true" << std::endl;
            }
            else
            {
                std::cout << "false" << std::endl;
            }
        }
        else
        {
            std::cout << "Comando inválido." << std::endl;
        }
    }

    // Imprime o estado final das árvores temporais
    teste.printTTC();

    // Fecha o arquivo
    arquivo.close();

    return 0; // Retorna 0 para indicar sucesso
}
