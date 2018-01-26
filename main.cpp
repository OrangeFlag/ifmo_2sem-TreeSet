#include <iostream>
#include <exception>
template<class Type>
class ISet
{
public:
    virtual void add(Type e) = 0;

    virtual void clear() = 0;

    virtual bool contains(Type e) = 0;

    virtual size_t size() = 0;

    virtual bool remove(Type e) = 0;
};



namespace TreeSet
{
    int8_t Black = 0;
    int8_t Red = 1;

    template<class Type>
    struct RBTEl
    {
        Type key;
        int8_t color; // 0 - black, 1 - red
        RBTEl *Left;
        RBTEl *Right;
        RBTEl *Parent;

        RBTEl(Type key_, int8_t color_ = Red, RBTEl *Parent_ = 0, RBTEl *Left_ = 0, RBTEl *Right_ = 0) :
                key(key_), color(color_), Left(Left_), Right(Right_),Parent(Parent_) {};
    };


    template<class Type>
    class RBT : ISet<Type> {
        RBTEl<Type> *Root;
        size_t size_;
        RBTEl<Type> *Null_el;

        void clear(RBTEl<Type>*& deleted)
        {
            if (deleted == Null_el)
                return;
            clear(deleted->Right);
            clear(deleted->Left);
            --size_;
            delete deleted;
            deleted = Null_el;
        };

        RBTEl<Type>* contains(Type e, RBTEl<Type>* Temp)
        {
            while (Temp != Null_el) {
                if (Temp->key < e) {
                    if (Temp->Right == Null_el)
                        return Temp;
                    Temp = Temp->Right;
                } else if (Temp->key > e) {
                    if (Temp->Left == Null_el)
                        return Temp;
                    Temp = Temp->Left;
                } else {
                    return Temp;
                };
            }
            return Null_el;
        };
        RBTEl <Type> * grantparent(RBTEl<Type>* Now)
        {
            return Now -> Parent -> Parent;
        };
        RBTEl<Type>* uncle(RBTEl<Type>* Now)
        {
            RBTEl<Type>* G = grantparent(Now);
            if (G->Left == Now->Parent)
            {
                return G->Right;
            }else
            {
                return G->Left;
            };

        };

        RBTEl<Type>* second_child(RBTEl<Type>* P, RBTEl<Type>* First)
        {
            if (P->Left == First)
            {
                return P->Right;
            }else if (P->Right == First)
            {
                return P->Left;
            } else
                return Null_el;
        };
        void rotate_left(RBTEl<Type>* Root)
        {
            //малые повороты
            RBTEl<Type>* Right = Root ->Right;
            if (Root == Null_el || Right == Null_el)
                return;
            Root -> Right = Right->Left;
            if (Right->Left != Null_el)
                Right->Left->Parent = Root;

            Right->Left = Root;
            Right->Parent = Root->Parent;
            Root->Parent = Right;

            if (Right->Parent != Null_el)
            {
                if (Right->Parent->Right == Root)
                {
                    Right->Parent->Right = Right;
                } else
                    Right->Parent->Left = Right;
            }
            else
            {
                this->Root = Right;
            }

        };

        void rotate_right(RBTEl<Type>* Root)
        {
            RBTEl<Type>* Left = Root -> Left;
            if (Root == Null_el || Left == Null_el)
                return;
            Root -> Left = Left->Right;
            if (Left->Right != Null_el)
                Left->Right->Parent = Root;

            Left->Right = Root;
            Left->Parent = Root->Parent;
            Root->Parent = Left;

            if (Left->Parent != Null_el)
            {
                if (Left->Parent->Right == Root)
                {
                    Left->Parent->Right = Left;
                } else
                    Left->Parent->Left = Left;
            }
            else
            {
                this->Root = Left;
            }
        };
        void autobalance_add(RBTEl<Type>* N)
        {
            if (N != Null_el)
            {
                if (N == this->Root)
                {
                    N->color = Black;
                    return;
                }
                RBTEl<Type>* P = N->Parent;
                if (P->color == Black)
                {
                    return;
                } else
                {
                    RBTEl<Type>* U = uncle(N);
                    RBTEl<Type>* G = grantparent(N);

                    if (U->color == Red)
                    {
                        U->color = Black;
                        P->color = Black;
                        G->color = Red;
                        autobalance_add(G);
                    } else
                    {
                        if (P->Right == N &&  G->Left == P)
                        {
                            rotate_left(P);
                            N = N->Left;
                        } else if (P->Left == N && G->Right == P)
                        {
                            rotate_right(P);
                            N = N->Right;
                        }

                        P = N->Parent;
                        G = P->Parent;

                        P->color = Black;
                        G->color = Red;


                        if (P -> Left == N && G -> Left == P)
                        {
                            rotate_right(G);
                        } else if (P->Right == N && G -> Right == P)
                        {
                            rotate_left(G);
                        };

                    }

                };

            };


        };
        void autobalance_rem(RBTEl<Type>* P, RBTEl<Type>* N)
        {
            if (P == Null_el)
                return;

            RBTEl<Type>* B = second_child(P, N);
            if (B->color == Red)
            {
                P->color = Red;
                B->color = Black;
                if (N == P->Right)
                {
                    rotate_right(P);
                } else
                {
                    rotate_left(P);
                }
            }

            B = second_child(P, N);
            if (B->color == Black &&
                B->Left->color == B->Right->color && B->Left->color == Black
                && P->color == Black)
            {
                B->color = Red;
                autobalance_rem(P->Parent, P);
                return;
            }

            B = second_child(P, N);
            if (B->color == Black &&
                B->Left->color == B->Right->color && B->Left->color == Black
                && P->color == Red)
            {
                B->color = Red;
                P->color = Black;
                return;
            }

            B = second_child(P, N);
            if  (B->color == Black) {
                if ((N == P->Left) &&
                    (B->Right->color == Black) &&
                    (B->Left->color == Red))
                {
                    B->color = Red;
                    B->Left->color = Black;
                    rotate_right(B);
                } else if ((N == P->Right) &&
                           (B->Left->color == Black) &&
                           (B->Right->color == Red))
                {
                    B->color = Red;
                    B->Right->color = Black;
                    rotate_left(B);
                }
            }

            B = second_child(P, N);
            B->color = P->color;
            P->color = Black;

            if (N == P->Left) {
                B->Right->color = Black;
                rotate_left(P);
            } else {
                B->Left->color = Black;
                rotate_right(P);
            }




        }
        RBTEl<Type>* min (RBTEl<Type>* N)
        {
            while(N -> Left != Null_el)
            {
                N = N->Left;
            };
            return N;
        };

        RBTEl<Type>* max (RBTEl<Type>* N)
        {
            while(N -> Right != Null_el)
            {
                N = N->Right;
            };
            return N;
        };
        bool remove(Type e, RBTEl<Type>* Root_)
        {
            RBTEl<Type>* Temp = contains(e, Root_);
            if (Temp != Null_el && Temp->key == e)
            {
                if (Temp->Left != Null_el && Temp->Right != Null_el)
                {
                    RBTEl<Type>* Z = min(Temp->Right);
                    Temp->key = Z->key;
                    return remove(Z->key, Z);

                } else if (Temp->Left != Null_el)
                {
                    RBTEl<Type>*P = Temp->Parent;
                    if (P->Left == Temp) {
                        P->Left = Temp->Left;
                    } else
                    if (P->Right == Temp){
                        P->Right = Temp->Left;
                    }
                    Temp->Left->Parent = P;
                    if (P == Null_el)
                        Root = Temp->Left;

                    if (Temp->color == Black)
                    {
                        if (Temp->Left->color == Red)
                        {
                            Temp->Left->color = Black;
                        }else
                            autobalance_rem(P, Temp->Left);
                    }
                    delete Temp;
                    --size_;
                    return true;
                } else
                {
                    RBTEl<Type>*P = Temp->Parent;
                    if (P->Left == Temp) {
                        P->Left = Temp->Right;
                    } else
                    if (P->Right == Temp){
                        P->Right = Temp->Right;
                    }

                    if (Temp->Right != Null_el)
                        Temp->Right->Parent = P;

                    if (P == Null_el)
                        Root = Temp->Right;

                    if (Temp->color == Black)
                    {
                        if (Temp->Right->color == Red)
                        {
                            Temp->Right->color = Black;
                        }else
                            autobalance_rem(P, Temp->Right);
                    }
                    delete Temp;
                    --size_;
                    return true;
                }
            };
            return false;

        };


    public:
        RBT() : Null_el(new RBTEl<Type>(0, Black)), Root(0), size_(0)
        {
            Null_el->Parent = Null_el;
            Null_el->Right = Null_el;
            Null_el->Left = Null_el;
            Root = Null_el;
        };
//-----------------------------------------------------only for test
        bool is_correct()
        {
            if (Root->color == Red)
            {
                throw 0;
            };
            try
            {
                is_correct(Root, 0);
            } catch (int e)
            {
                throw 0;
            }
            return true;
        }
        int is_correct(RBTEl<Type>* Now, int Black_length)
        {
            if (Now == Null_el)
                return Black_length;
            if (Now->color == Black)
                ++Black_length;
            else if (Now->Parent->color == Red) {
                throw 0;
            }
            int a = 0;
            int b = 0;
            try
            {
            a = is_correct(Now->Left, Black_length);
            b = is_correct(Now->Right, Black_length);
            }
            catch (int e)
            {
                throw 0;
            }
            if (a != b) {
                throw 0;
            }
            return a;
        }
//----------------------------------------------------- ~only for test
        void clear()
        {
            clear(Root);
        };

        bool contains(Type e)
        {
            RBTEl<Type> *Temp = contains(e, Root);
            return (Temp != Null_el && Temp->key == e);
        };

        size_t size()
        {
            return size_;
        };
        void add(Type e)
        {
            if (Root == Null_el) {
                Root = new RBTEl<Type>(e, Black, Null_el, Null_el, Null_el);
                ++size_;
                return;
            };

            RBTEl<Type>* Temp = contains(e, Root);
            if (Temp != Null_el && Temp->key != e) {
                if (Temp->key < e) {
                    Temp->Right = new RBTEl<Type>(e, Red, Temp, Null_el, Null_el);
                    autobalance_add(Temp->Right);
                } else {
                    Temp->Left = new RBTEl<Type>(e, Red, Temp, Null_el, Null_el);
                    autobalance_add(Temp->Left);
                };
                ++size_;
            };


        };

        bool remove(Type e)
        {
            return remove(e, Root);
        };


        //iterator

        class iterator: public std::iterator<std::input_iterator_tag, Type>
        {
            friend class RBT<Type>;
        private:
            iterator(RBTEl<Type>* point, RBTEl<Type>* Null_el, RBTEl<Type>* last):point(point), Null_el(Null_el), last(last){};
        public:
            iterator(const iterator &it):point(it.point), Null_el(it.Null_el), last(it.last){};
            bool operator!=(iterator const& other) const
            {
                return point != other.point;
            };
            bool operator==(iterator const& other) const
            {
                return point == other.point;
            };
            iterator& operator++()
            {
                if (point->Left != Null_el && last != point->Left && last != point->Right)
                {
                    last = point;
                    point = point->Left;
                    return *this;
                } else if(point->Right != Null_el && last != point->Right)
                {
                    last = Null_el;
                    point = point->Right;
                    while (point -> Left != Null_el)
                    {
                        point = point->Left;
                    };
                    return *this;
                } else
                {
                    last = point;
                    point = point->Parent;
                    while (point->Right == Null_el || point->Right == last)
                    {
                        last = point;
                        point = point->Parent;
                        if (point == Null_el)
                        {
                            last = Null_el;
                            break;
                        }
                    }
                    return *this;
                }
            };
            RBTEl<Type> operator*() const
            {
                return *point;
            };
        public:
            RBTEl<Type>* point;
            RBTEl<Type>* last;
            RBTEl<Type>* Null_el;
        };

        iterator begin()
        {
            return iterator(min(Root), Null_el, Null_el);
        };
        iterator end()
        {
            return iterator(Null_el, Null_el, Null_el);
        };
    };


}

using namespace TreeSet;
using namespace std;
int main() {
    RBT<int> Set;
    string com;
    int operand;
    cin >> com;
    int i = 0;
    while (com != "")
    {
        switch (com[0])
        {
            case 'a':
                cin >> operand;
                Set.add(operand);
                break;
            case 'e':
                cin >> operand;
                cout << Set.contains(operand) << endl;
                break;
            case 'r':
                cin >> operand;
                cout << Set.remove(operand) << endl;
                break;
            case 's':
                cout << Set.size() << endl;
                break;
            case 'c':
                Set.clear();
                break;
            case 'i':
                for (auto it = Set.begin(); it != Set.end(); ++it)
                {
                    cout << (*it).key << " ";
                }
                cout << endl;
                break;
        };
        Set.is_correct();
        com = "";
        cin >> com;
        ++i;

    }
    return 0;
}