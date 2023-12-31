#include "PolynomialList.h"
#include <iostream>
#include <algorithm>
#include <fstream>
#include <cmath>
#include <assert.h>

#define EPSILON 1.0e-10	// zero double

using namespace std;

PolynomialList::PolynomialList(const PolynomialList& other) {
    //TO DO
    m_Polynomial = other.m_Polynomial;
}

PolynomialList::PolynomialList(const string& file) {
    //TO DO
    ReadFromFile(file);
}

PolynomialList::PolynomialList(const double* cof, const int* deg, int n) {
    //TO DO
    for (int i = 0; i < n; i++)
        AddOneTerm(Term(deg[i], cof[i]));
}

PolynomialList::PolynomialList(const vector<int>& deg, const vector<double>& cof) {
    //TO DO
    assert(deg.size() == cof.size());

    for (size_t i = 0; i < deg.size(); i++)
        AddOneTerm(Term(deg[i], cof[i]));
}

double PolynomialList::coff(int i) const {
    //TO DO
    for (const Term& term : m_Polynomial) {
        if (term.deg > i)
            break;
        if (term.deg == i)
            return term.cof;
    }

    return 0.;
}

double& PolynomialList::coff(int i) {
    //TO DO
    return AddOneTerm(Term(i, 0)).cof;
}

void PolynomialList::compress() {
    //TO DO
    auto p = m_Polynomial.begin();
    while (p != m_Polynomial.end()) {
        if (fabs((*p).cof) < EPSILON)
            p = m_Polynomial.erase(p);
        else
            p++;
    }
}

PolynomialList PolynomialList::operator+(const PolynomialList& right) const {
    //TO DO
    PolynomialList temp(right);
    for (const auto& term : m_Polynomial)
        temp.AddOneTerm(term);

    temp.compress();
    return temp;
}

PolynomialList PolynomialList::operator-(const PolynomialList& right) const {
    //TO DO
    PolynomialList temp(right);
    for (const auto& term : m_Polynomial)
        temp.AddOneTerm(Term(term.deg, -term.cof));
    temp.compress();
    return temp;
}

PolynomialList PolynomialList::operator*(const PolynomialList& right) const {
    //TO DO
    PolynomialList temp;
    for (const auto& term1 : m_Polynomial) {
        for (const auto& term2 : right.m_Polynomial) {
            double cof = term1.cof * term2.cof;
            int deg = term1.deg + term2.deg;
            temp.AddOneTerm(Term(deg, cof));
        }
    }

    return temp;
}

PolynomialList& PolynomialList::operator=(const PolynomialList& right) {
    //TO DO
    m_Polynomial = right.m_Polynomial;
    return *this;
}

void PolynomialList::Print() const {
    //TO DO
    auto p = m_Polynomial.begin();
    if (p == m_Polynomial.end()) {
        cout << "0" << endl;
        return;
    }

    for (; p != m_Polynomial.end(); p++)
    {
        if (p != m_Polynomial.begin())
        {
            cout << " ";
            if (p->cof > 0)
                cout << "+";
        }

        cout << p->cof;

        if (p->deg > 0)
            cout << "x^" << p->deg;
    }
    cout << endl;
}

bool PolynomialList::ReadFromFile(const string& file) {
    //TO DO
    m_Polynomial.clear();

    ifstream fp;
    fp.open(file.c_str());
    if (!fp.is_open()) {
        cout << "file [" << file << "] opens failed" << endl;
        return false;
    }

    char c;
    int n;
    fp >> c>> n;
    for (int i = 0; i < n; i++) {
        Term nd;
        fp >> nd.deg;
        fp >> nd.cof;

        AddOneTerm(nd);
    }

    fp.close();

    return true;
}

PolynomialList::Term& PolynomialList::AddOneTerm(const Term& term) {
    //TO DO
    auto p = m_Polynomial.begin();
    for (; p != m_Polynomial.end(); p++) {
        if (p->deg == term.deg) {
            p->cof += term.cof;
            return *p;
        }

        if (p->deg > term.deg)
            break;
    }
    return *m_Polynomial.insert(p, term);
}
