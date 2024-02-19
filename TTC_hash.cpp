#include <bits/stdc++.h>

using namespace std;

// Definição da classe RTuple
class RTuple
{
public:
    int u;
    int v;
    int Tminus;
    int Tplus;

    RTuple() : RTuple(-1, -1, -1, -1){};
    RTuple(int a, int b, int c, int d) : u(a), v(b), Tminus(c), Tplus(d){};

    // Método para verificar se a RTuple está vazia
    bool empty()
    {
        return (Tminus == -1 && Tplus == -1);
    }

    // Método para imprimir uma RTuple
    void print_tuple()
    {
        cout << "(" << u << ", " << v << ", " << Tminus << ", " << Tplus << ") ";
    }

    // Método para verificar se a RTuple contém outra RTuple
    bool contains(RTuple other1, RTuple other2)
    {
        if (other1.empty())
        {
            return false;
        }
        else if (other2.empty())
        {
            return (Tminus >= other1.Tminus);
        }

        return (Tminus >= other1.Tminus && Tplus <= other2.Tplus);
    }

    // Operadores de comparação para ordenação
    bool operator<(const RTuple &other) const
    {
        if (Tminus != other.Tminus)
        {
            return Tminus < other.Tminus;
        }
        else
        {
            return Tplus < other.Tplus;
        }
    };

    bool operator<=(const RTuple &other) const
    {
        if (Tminus != other.Tminus)
        {
            return Tminus <= other.Tminus;
        }
        else
        {
            return Tplus <= other.Tplus;
        }
    };

    bool operator!=(const RTuple &other) const
    {
        return (Tminus != other.Tminus || Tplus != other.Tplus);
    }

    bool operator==(const RTuple &other) const
    {
        return (Tminus == other.Tminus || Tplus == other.Tplus);
    }
};

// Estrutura de comparação para o conjunto de RTuples
struct SetCompare
{
    bool operator()(RTuple lhs, RTuple rhs) const
    {
        return lhs < rhs;
    }
};

// Hashing para pares de inteiros (usado para a tabela hash)
struct pair_hash
{
    template <class T1, class T2>
    std::size_t operator()(const std::pair<T1, T2> &p) const
    {
        auto h1 = std::hash<T1>{}(p.first);
        auto h2 = std::hash<T2>{}(p.second);

        // Combina os valores de hash de uma maneira adequada para o caso
        return h1 ^ h2;
    }
};

// Definição da classe Temporal_Graph
class Temporal_Graph
{
public:
    int tam;
    unordered_map<pair<int, int>, set<RTuple, SetCompare>, pair_hash> hashTable;
    int delta;

    Temporal_Graph() : Temporal_Graph(1){};

    Temporal_Graph(int a) : delta(1), hashTable({{}}), tam(a){};

    // Métodos auxiliares

    int get_node_index(const std::string &node_str) const
    {
        // Método para obter o índice do nó a partir de sua representação (string ou inteiro)
        if (node_str.length() == 1 && isalpha(node_str[0]))
        {
            return node_str[0] - 'a';
        }
        else
        {
            return std::stoi(node_str);
        }
    }

    set<int> neighboursIn(int a)
    {
        // Método para obter vizinhos de entrada do nó 'a'
        set<int> nIn{};
        for (int x = 0; x < tam; x++)
        {
            if (!hashTable[{x, a}].empty())
            {
                nIn.insert(x);
            }
        }
        return nIn;
    }

    set<int> neighboursOut(int a)
    {
        // Método para obter vizinhos de saída do nó 'a'
        set<int> nOut{};
        for (int x = 0; x < tam; x++)
        {
            if (!hashTable[{a, x}].empty())
            {
                nOut.insert(x);
            }
        }
        return nOut;
    }

    void insert(int a, int b, int t1, int t2)
    {
        // Método para inserir uma nova RTuple na tabela hash
        RTuple newTuple(a, b, t1, t2);

        if (hashTable[{a, b}].empty())
        {
            set<RTuple, SetCompare> newSet = {newTuple};
            hashTable[make_pair(a, b)] = newSet;
        }
        else
        {
            // Removendo redundâncias
            auto l1 = find_prev(a, b, t2);
            if (l1.Tplus < t2)
            {
                if (l1.Tminus >= t1)
                {
                    return;
                }
                else
                {
                    l1 = find_next(a, b, l1.Tplus);
                }
            }

            auto l2 = find_next(a, b, t1);

            if (!l2.empty() && l2.Tminus > t1)
            {
                l2 = find_prev(a, b, l2.Tplus);
            }

            set<RTuple, SetCompare>::iterator it1, it2;

            if (l1.empty())
            {
                it1 = hashTable[{a, b}].end();
            }
            else
            {
                it1 = hashTable[{a, b}].find(l1);
            }

            if (l2.empty())
            {
                it2 = hashTable[{a, b}].end();
            }
            else
            {
                it2 = hashTable[{a, b}].find(l2);
            }

            set<RTuple, SetCompare> elementsToRemove;

            while (it1 != it2 and it1 != hashTable[{a, b}].end());
            {
                auto elem = *it1;
                if (elem.contains(l1, l2))
                {
                    elementsToRemove.insert(elem);
                }

                ++it1;
            }

            for (auto elem : elementsToRemove)
            {
                hashTable[{a, b}].erase(elem);
            }
            hashTable[{a, b}].insert(newTuple);
        }
    }

    RTuple find_next(int u, int v, int t)
    {
        // Método para encontrar a próxima RTuple a partir de um tempo 't'
        set<RTuple, SetCompare> tuplas = hashTable[{u, v}];
        RTuple next(u, v, t, -1);
        auto it = tuplas.lower_bound(next);

        if (it == tuplas.end())
        {
            return RTuple(u, v, -1, -1);
        }
        else
        {
            return *it;
        }
    }

    RTuple find_prev(int u, int v, int t)
    {
        // Método para encontrar a RTuple mais recente antes de um tempo 't'
        set<RTuple, SetCompare> tuplas = hashTable[{u, v}];
        RTuple latest(u, v, -1, -1);

        for (const auto &tupla : tuplas)
        {
            if (tupla.Tplus <= t)
            {
                latest = tupla;
            }
        }
        return latest;
    }

    // Funções principais

    void add(string u_str, string v_str, int t)
    {
        // Método para adicionar um novo contato entre 'u' e 'v' no tempo 't'
        int a = get_node_index(u_str);
        int b = get_node_index(v_str);
        insert(a, b, t, t + delta);
        set<pair<int, int>> aux;
        set<int> neigh_a = neighboursIn(a);
        set<int> neigh_b = neighboursOut(b);

        // Vizinhos de entrada
        for (auto x : neigh_a)
        {
            RTuple prev = find_prev(x, a, t);
            if (!prev.empty())
            {
                insert(x, b, prev.Tminus, t + delta);
                aux.insert({x, prev.Tminus});
            }
        }

        // Vizinhos de saída
        for (auto x : neigh_b)
        {
            RTuple next = find_next(b, x, t + delta);
            if (next.Tminus > t + 1)
            {
                next = RTuple(b, x, -1, -1);
            };
            if (!next.empty())
            {
                insert(a, x, t, next.Tplus);
                for (auto tupla : aux)
                {
                    if (tupla.first != x)
                    {
                        insert(tupla.first, x, tupla.second, next.Tplus);
                    }
                }
            }
        }
    }

    bool can_reach(string u_str, string v_str, int t1, int t2)
    {
        // Método para verificar se é possível alcançar 'v' a partir de 'u' entre os tempos 't1' e 't2'
        int a = get_node_index(u_str);
        int b = get_node_index(v_str);
        set<RTuple, SetCompare> tuplas = hashTable[{a, b}];
        RTuple next = find_next(a, b, t1);
        RTuple prev = find_prev(a, b, t2);
        if ((!next.empty() && next.Tplus <= t2) || (!prev.empty() && prev.Tplus >= t1))
        {
            return true;
        }
        return false;
    }

    bool is_connected(int t1, int t2)
    {
        // Método para verificar se o grafo é conectado entre os tempos 't1' e 't2'
        // Implementação pendente
        // Pode ser adicionada de acordo com os requisitos específicos
    }

    void print_connections()
    {
        // Método para imprimir as conexões no grafo
        for (int x = 0; x < tam; x++)
        {
            for (int y = 0; y < tam; y++)
            {
                if (!hashTable[{x, y}].empty())
                {
                    int u = x;
                    int v = y;
                    cout << "\nSet[" << u << "][" << v << "]: ";
                    for (auto t : hashTable[{x, y}])
                    {
                        cout << "(" << t.Tminus << ", " << t.Tplus << "); ";
                    }
                }
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
    Temporal_Graph teste(numeroDeVertices);

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
            teste.add(u, v, t);
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
    teste.print_connections();

    // Fecha o arquivo
    arquivo.close();

    return 0; // Retorna 0 para indicar sucesso
}
