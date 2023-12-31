#include "PolynomialMap.h"

#include <iostream>
#include <fstream>
#include <cassert>
#include <cmath>

#define EPSILON 1.0e-10	// zero double

using namespace std;

PolynomialMap::PolynomialMap(const PolynomialMap& other) {
    //TO DO
    m_Polynomial = other.m_Polynomial;
}

PolynomialMap::PolynomialMap(const string& file) {
    //TO DO
    ReadFromFile(file);
}

PolynomialMap::PolynomialMap(const double* cof, const int* deg, int n) {
    //TO DO
    for (int i = 0; i < n; i++)
        coff(deg[i]) = cof[i];
}

PolynomialMap::PolynomialMap(const vector<int>& deg, const vector<double>& cof) {
    //TO DO
    assert(deg.size() == cof.size());

    for (size_t i = 0; i < deg.size(); i++)
        coff(deg[i]) = cof[i];
}

double PolynomialMap::coff(int i) const
{
    //TO DO
    auto tar = m_Polynomial.find(i);
    if (tar == m_Polynomial.end())
        return 0.;

    return tar->second;
}

double& PolynomialMap::coff(int i) {
    //TO DO
    return m_Polynomial[i];
}

void PolynomialMap::compress() {
    //TO DO
    auto p = m_Polynomial.begin();
    while (p != m_Polynomial.end()) {
        if (fabs((*p).second) < EPSILON)
            p = m_Polynomial.erase(p);
        else
            p++;
    }
}

PolynomialMap PolynomialMap::operator+(const PolynomialMap& right) const {
    //TO DO
    PolynomialMap poly(right);
    for (const auto& term : m_Polynomial)
        poly.coff(term.first) += term.second;

    poly.compress();
    return poly;
}

PolynomialMap PolynomialMap::operator-(const PolynomialMap& right) const {
    //TO DO
    PolynomialMap poly(right);
    for (const auto& term : m_Polynomial)
        poly.coff(term.first) -= term.second;

    poly.compress();
    return poly;
}

PolynomialMap PolynomialMap::operator*(const PolynomialMap& right) const {
    //TO DO
    PolynomialMap poly;
    for (const auto& term1 : m_Polynomial) {
        for (const auto& term2 : right.m_Polynomial) {
            int deg = term1.first + term2.first;
            double cof = term1.second * term2.second;
            poly.coff(deg) += cof;
        }
    }
    return poly;
}

PolynomialMap& PolynomialMap::operator=(const PolynomialMap& right) {
    //TO DO
    m_Polynomial = right.m_Polynomial;
    return *this;
}

void PolynomialMap::Print() const {
    //TO DO
    auto p = m_Polynomial.begin();
    if (p == m_Polynomial.end()) {
        cout << "0" << endl;
        return;
    }

    for (; p != m_Polynomial.end(); p++) {
        if (p != m_Polynomial.begin()) {
            cout << " ";
            if (p->second > 0)
                cout << "+";
        }

        cout << p->second;

        if (p->first > 0)
            cout << "x^" << p->first;
    }
    cout << endl;
}

bool PolynomialMap::ReadFromFile(const string& file)
{
    //TO DO
    m_Polynomial.clear();

    ifstream fp;
    fp.open(file.c_str());
    if (!fp.is_open()) {
        cout <<"file [" << file << "] opens failed" << endl;
        return false;
    }

    char ch;
    int n;
    fp >> ch;
    fp >> n;
    for (int i = 0; i < n; i++) {
        int deg;
        double cof;
        fp >> deg;
        fp >> cof;
        coff(deg) = cof;
    }

    fp.close();

    return true;
}
