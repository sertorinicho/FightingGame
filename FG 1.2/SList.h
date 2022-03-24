#ifndef SLIST_H
#define SLIST_H

#include <stdexcept>

template <class T>
class SList {           // Classe Contenitore Personalizzato (simile ad una Forward_List)
private:
    class Node {        // Classe Nodo Annidata
    public:
        T info;
        Node* next;
        Node(const T& =T(), Node* =nullptr);
        ~Node();
    };
    Node* first;
    Node* last;
    static Node* copy(Node*, Node*&);   // Copia profonda


public:
    SList(Node* =nullptr);              // Costruttore di default
    SList(const SList&);                // Costruttore di copia profonda
    ~SList();                           // Distruttore Profondo
    SList& operator=(const SList&);     // Assegnazione Profonda
    void push_front(const T&);          // Inserimento in cima alla lista
    void push_back(const T&);           // Inserimento in fondo alla lista
    T pop_front();                      // Restituisce una copia del campo info di first, distrugge il nodo first e sposta first al nodo successivo
    T pop_back();                       // Restituisce una copia del campo info di last, distrugge il nodo last e sposta last al nodo precedente
    void merge(SList&);                 // Aggiunge in coda alla lista di invocazione la lista passata per riferimento, e pulisce la seconda

    // SList iterator
    class iterator {
        friend class SList;
    private:
        Node* ptr;
        bool PTE;                       // true sse iteratore past-the-end
        iterator(Node*, bool=false);

    public:
        iterator();
        T& operator*() const;
        T* operator->() const;
        T& operator[](int) const;
        iterator& operator++();
        iterator operator++(int);
        bool operator==(const iterator&) const;
        bool operator!=(const iterator&) const;
    };

    // SList const_iterator
    class const_iterator {
        friend class SList;
    private:
        Node* ptr;
        bool PTE;                       // true sse iteratore past-the-end
        const_iterator(Node*, bool=false);


    public:
        const_iterator();
        const_iterator(iterator);       // conversione implicita iterator->const_iterator
        const T& operator*() const;
        const T* operator->() const;
        const T& operator[](int) const;
        const_iterator& operator++();
        const_iterator operator++(int);
        bool operator==(const const_iterator&) const;
        bool operator!=(const const_iterator&) const;
    };

public:
    iterator begin();               // Restituisce un iteratore che punta al primo elemento
    iterator end();                 // Restituisce un iteratore che punta all'elemento PTE
    const_iterator begin() const;   // Restituisce un const_iteratore che punta al primo elemento
    const_iterator end() const;     // Restituisce un const_iteratore che punta all'elemento PTE
    const_iterator cbegin() const;  // Restituisce un const_iteratore che punta al primo elemento
    const_iterator cend() const;    // Restituisce un const_iteratore che punta all'elemento PTE

    iterator insert_after(const_iterator cit, const T&);                // Aggiunge un elemento dopo l'elemento puntato dall'iteratore. Se non riesce, ritorna un iteratore che punta a cit
    iterator insert_after(const_iterator, unsigned int, const T&);      // Aggiunge n elementi dopo l'elemento puntato dall'iteratore.
    iterator erase_after(const_iterator cit);                           // Cancella un elemento dopo l'elemento puntato dall'iteratore. Se non rimuove l'elemento ritorna un iteratore che punta a cit, se rimuove l'ultimo el ritorna end
    iterator erase_after(const_iterator, unsigned int n);               // Cancella n elementi dopo l'elemento puntato dall'iteratore.
    void clear();                                                       // Rimuove tutti gli elementi dalla sList

};

template <class T>
SList<T>::Node::Node(const T& i, Node* n) : info(i), next(n) {}

template <class T>
SList<T>::Node::~Node() {
    delete next;
}

template <class T>
SList<T>::SList(SList::Node* f) : first(f), last(f) {}

template <class T>
SList<T>::SList(const SList& sl) : first(copy(sl.first, last)) {}   // last inizializzato per riferimento

template <class T>
SList<T>::~SList() {
    delete first;
}

template <class T>
SList<T>& SList<T>::operator=(const SList& sl) {
    if(this != &sl)
    {
        delete first;
        first = copy(sl.first, last);
    }
    return *this;
}

//
template <class T>
typename SList<T>::Node* SList<T>::copy(SList::Node* f, SList::Node*& l) {
    if(!f)
        return l = nullptr;

    Node* n = new Node(f->info, copy(f->next, l));
    if(!n->next)
        l = n;
    return n;
}

template <class T>
void SList<T>::push_front(const T& t) {
    first = new Node(t, first);
    if(!last)
        last = first;
}

template <class T>
void SList<T>::push_back(const T& t) {
    if(!last)
        first = last = new Node(t);
    else
        last = last->next = new Node(t);
}
template <class T>
T SList<T>::pop_front() {
    if(first == 0)
        throw(std::logic_error("ERRORE: pop su lista vuota"));
    T in = first->info;
    if(first == last) {
        delete first;
        first = 0;
        last = 0;
    }
    else {
        Node* aux = first->next;
        first->next = 0;
        delete first;
        first = aux;
    }
    return in;
}

template <class T>
T SList<T>::pop_back() {
    if(first == 0)
        throw(std::logic_error("ERRORE: pop su lista vuota"));
    T in = last->info;
    if(first == last) {
        delete first;
        first = 0;
        last = 0;
    }
    else {
        Node* pre = first;
        while(pre->next != last)
            pre = pre->next;
        delete last;
        pre->next = 0;
        last = pre;
    }
    return in;
}

template <class T>
void SList<T>::merge(SList& sl) {
    if(!first) {
        first = sl.first;
        last = sl.last;
    }
    else if(first && sl.first) {
        last->next = sl.first;
        last = sl.last;
    }
    // if( !sl.first || (!first && !sl.first)) non serve fare niente
    sl.first = 0;
    sl.last = 0;
}

// iterator

template <class T>
SList<T>::iterator::iterator(Node* p, bool PastTE) : ptr(p), PTE(PastTE) {}

template <class T>
SList<T>::iterator::iterator() : ptr(nullptr), PTE(false) {}

template <class T>
T& SList<T>::iterator::operator*() const {
    return ptr->info;
}

template <class T>
T* SList<T>::iterator::operator->() const {
    return &(ptr->info);
}

// Non si controlla se ci si trova out of bounds
template <class T>
T& SList<T>::iterator::operator[](int k) const {
    iterator aux(ptr);
    for(int i = 0; i<k; i++)
        aux.ptr = (aux.ptr)->next;
    return aux.ptr->info;
}

template <class T>
typename SList<T>::iterator& SList<T>::iterator::operator++() {
    if(ptr){
        if(!PTE) {
            if(!ptr->next) {
                ++ptr;
                PTE = true;
            }
            else
                ptr = ptr->next;
        }
    }
    return *this;
}

template <class T>
typename SList<T>::iterator SList<T>::iterator::operator++(int) {
    iterator aux(ptr, PTE);

    if(ptr){
        if(!PTE) {
            if(!ptr->next) {
                ++ptr;
                PTE = true;
            }
            else
                ptr = ptr->next;
        }
    }
    return aux;
}

template <class T>
bool SList<T>::iterator::operator==(const iterator& it) const {
    return ptr == it.ptr;
}

template <class T>
bool SList<T>::iterator::operator!=(const iterator& it) const {
    return ptr != it.ptr;
}


// const_iterator

template <class T>
SList<T>::const_iterator::const_iterator(Node* p, bool PastTE) : ptr(p), PTE(PastTE) {}

template <class T>
SList<T>::const_iterator::const_iterator(iterator it) : ptr(it.ptr), PTE(it.PTE) {}

template <class T>
SList<T>::const_iterator::const_iterator() : ptr(nullptr), PTE(false) {}

template <class T>
const T& SList<T>::const_iterator::operator*() const {
    return ptr->info;
}

template <class T>
const T* SList<T>::const_iterator::operator->() const {
    return &(ptr->info);
}

// Non si controlla se ci si trova out of bounds
template <class T>
const T& SList<T>::const_iterator::operator[](int k) const {
    iterator aux(ptr);
    for(int i = 0; i<k; i++)
        aux.ptr = (aux.ptr)->next;
    return aux.ptr->info;
}

template <class T>
typename SList<T>::const_iterator& SList<T>::const_iterator::operator++() {
    if(ptr){
        if(!PTE) {
            if(!ptr->next) {
                ++ptr;
                PTE = true;
            }
            else
                ptr = ptr->next;
        }
    }
    return *this;
}

template <class T>
typename SList<T>::const_iterator SList<T>::const_iterator::operator++(int) {
    iterator aux(ptr, PTE);

    if(ptr){
        if(!PTE) {
            if(!ptr->next) {
                ++ptr;
                PTE = true;
            }
            else
                ptr = ptr->next;
        }
    }
    return aux;
}

template <class T>
bool SList<T>::const_iterator::operator==(const const_iterator& it) const {
    return ptr == it.ptr;
}

template <class T>
bool SList<T>::const_iterator::operator!=(const const_iterator& it) const {
    return ptr != it.ptr;
}

template <class T>
typename SList<T>::iterator SList<T>::begin() {
    return first;
}

template <class T>
typename SList<T>::iterator SList<T>::end() {
    return !last ? nullptr : iterator(last+1, true);
}

template <class T>
typename SList<T>::const_iterator SList<T>::begin() const {
    return first;
}

template <class T>
typename SList<T>::const_iterator SList<T>::end() const {
    return !last ? nullptr : iterator(last+1, true);
}

template <class T>
typename SList<T>::const_iterator SList<T>::cbegin() const {
    return first;
}

template <class T>
typename SList<T>::const_iterator SList<T>::cend() const {
    return !last ? nullptr : iterator(last+1, true);
}

template <class T>
typename SList<T>::iterator SList<T>::insert_after(const_iterator cit, const T& val) {

    if(!cit.ptr) // && first
        return cit.ptr;

    cit.ptr->next = new Node(val, cit.ptr->next);
    if(cit.ptr == last)
        last = cit.ptr->next;
    return cit.ptr->next;
}

template <class T>
typename SList<T>::iterator SList<T>::insert_after(const_iterator cit, unsigned int n, const T& val) {
    if(!cit.ptr)
        return cit.ptr;

    Node* p = cit.ptr;
    for(unsigned int i = 0; i < n; i++) {
         p->next = new Node(val, p->next);
         p = p->next;
    }
    if(last == cit.ptr)
        last = p;
    return p;
}

template <class T>
typename SList<T>::iterator SList<T>::erase_after(const_iterator cit) {
    if(!cit.ptr || cit.ptr == last)
        return cit.ptr;

    Node* p = cit.ptr->next;
    if(p == last) {
        last = cit.ptr;
        cit.ptr->next = 0;
        delete p;
        return end();
    }
    else {
        cit.ptr->next = p->next;
        p->next = 0;
        delete p;
        return cit.ptr->next;
    }

}

template <class T>
typename SList<T>::iterator SList<T>::erase_after(const_iterator cit, unsigned int n) {
    if(!cit.ptr)
        return cit.ptr;

    // iterator della lista

    Node* first_toDel = nullptr;
    Node* p = nullptr;
    if(cit.ptr->next)
       first_toDel = p = cit.ptr->next;
    for(unsigned int i = 1; i < n && p; i ++) {
        p = p->next;
    }
    if(p && p != last) {
        cit.ptr->next = p->next;
        p->next = 0;
        delete first_toDel;
        return cit.ptr->next;
    }
    else {
        cit.ptr->next = 0;
        last = cit.ptr;
        delete first_toDel;
        return end();
    }


}

template <class T>
void SList<T>::clear() {
    //if(first)
    delete first;
    first = last = 0;
}

#endif // SLIST_H

