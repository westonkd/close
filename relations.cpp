/***************************************************************************
* Program:
*    Relations as Connection Matrices
* Author:
*    You
* Summary:
*    Represents relations as connection (zero-one) matrices, and provides
*    functionality for testing properties of relations.
*
***************************************************************************/

#include <cmath>
#include <iostream>
#include <fstream>
#include <iomanip>
using namespace std;

class Relation operator*(Relation& r1, Relation& r2);

class Relation
{
private:
   bool** mMatrix;
   int mSize;

   void init()
   {
      mMatrix = new bool*[mSize];
      for (int i = 0; i < mSize; i++)
      {
         mMatrix[i] = new bool[mSize];
      }
   }

public:
   Relation(int size)
   {
      mSize = size;
      init();
   }

   Relation& operator=(const Relation& rtSide)
   {
      if (this == &rtSide)
      {
         return *this;
      }
      else
      {
         mSize = rtSide.mSize;
         for (int i = 0; i < mSize; i++)
         {
            delete [] mMatrix[i];
         }
         delete [] mMatrix;
         init();
         for (int col = 0; col < mSize; col++)
         {
            for (int row = 0; row < mSize; row++)
            {
               mMatrix[col][row] = rtSide[col][row];
            }
         }
      }
      return *this;
   }

   Relation(const Relation& relation)
   {
      mSize = relation.getConnectionMatrixSize();
      init();
      *this = relation;
   }

   ~Relation()
   {
      for (int i = 0; i < mSize; i++)
      {
         delete [] mMatrix[i];
      }
      delete [] mMatrix;
   }

   int getConnectionMatrixSize() const
   {
      return mSize;
   }

   bool* operator[](int row) const
   {
      return mMatrix[row];
   }

   bool operator==(const Relation& relation)
   {
      int size = relation.getConnectionMatrixSize();
      if (mSize != size)
      {
         return false;
      }
      for (int i = 0; i < size; i++)
      {
         for (int j = 0; j < size; j++)
         {
            if (mMatrix[i][j] != relation[i][j])
            {
               return false;
            }
         }
      }
      return true;
   }

   /****************************************************************************
    *                                                                          *
    *  In the next seven functions replace the "return false;" with your code. *
    *  Note: you may have to implement some of these functions outside of this *
    *  class definition (like is done with the describe function).             *
    *                                                                          *
    ***************************************************************************/
   bool isReflexive()
   {
      //if we ever hit a '0' in the diagonal, return false
      for (int i = 0; i < mSize; i++)
         if (!mMatrix[i][i])
            return false;
      return true; 
   }

   bool isIrreflexive()
   {
      //if we ever hit a '1' in the diagonal, return true
      for (int i = 0; i < mSize; i++)
         if (mMatrix[i][i])
            return false;
      return true;
   }

   bool isNonreflexive()
   {
      return !isReflexive() && !isIrreflexive();
   }

   bool isSymmetric()
   {
      //check every entry
      for (int i = 0; i < mSize; i++)
         for (int j = 0; j < mSize; j++)
            //if the entry is true, make sure its inverse is as well
            if (mMatrix[i][j] && !mMatrix[j][i])
               return false; 

      return true;
   }

   bool isAntisymmetric()
   {
      //check every value
      for (int i = 0; i < mSize; i++)
         for (int j = 0; j < mSize; j++)
            //if the value is true, make sure the inverse value is NOT
            if (mMatrix[i][j] && mMatrix[j][i] && i != j)
               return false;

      return true;
   }

   bool isAsymmetric()
   {
      //Asymmetric = antisymmetric and irreflexive
      return isAntisymmetric() && isIrreflexive();
   }

   bool isTransitive();

   void describe();
};



ostream& operator<<(ostream& os, const Relation& relation)
{
   int n = relation.getConnectionMatrixSize();
   for (int i = 0; i < n; i++)
   {
      for (int j = 0; j < n; j++)
      {
         os << relation[i][j] << " ";
      }
      os << endl;
   }
   return os;
}

istream& operator>>(istream& is, Relation& relation)
{
   int n = relation.getConnectionMatrixSize();
   for (int i = 0; i < n; i++)
   {
      for (int j = 0; j < n; j++)
      {
         is >> relation[i][j];
      }
   }
   return is;
}

bool Relation::isTransitive()
{
   Relation mSquared = *this * *this;
   
   for (int i = 0; i < mSize; i++)
      for (int j = 0; j < mSize; j++)
         if (mSquared[i][j] && !mMatrix[i][j])
            return false;

   return true;
}

Relation operator*(Relation& r1, Relation& r2)
{
   //Boolean square of r1
   Relation product = Relation(r1);

   int size = r1.getConnectionMatrixSize();
   
   //Loop through all columns
   for(int col = 0; col < size; col++)
   {
      //Loop through every row
      for(int row = 0; row < size; row++)
      {
         //Sum, or boolean OR of each col
         bool sum = false;

         //check every element
         for(int i = 0; i < size; i++)
         {
            //AND together each inverse pair
            if(r1[i][row] && r1[col][i])
            {
               //if one of the ANDed pairs is true, all the OR will be as well
               sum = true;
               break;
            }
         }
         product[col][row] = sum;
      }
   }

   //return the new relation matrix
   return product;
}

void Relation::describe()
{
   cout << "\nThe relation represented by the " << mSize << "x" << mSize << " matrix\n";
   cout << *this << "is\n";
   cout << (isReflexive() ? "" : "NOT ") << "Reflexive;\n";
   cout << (isIrreflexive() ? "" : "NOT ") << "Irreflexive;\n";
   cout << (isNonreflexive() ? "" : "NOT ") << "Nonreflexive;\n";
   cout << (isSymmetric() ? "" : "NOT ") << "Symmetric;\n";
   cout << (isAntisymmetric() ? "" : "NOT ") << "Antisymmetric;\n";
   cout << (isAsymmetric() ? "" : "NOT ") << "Asymmetric; and\n";
   cout << (isTransitive() ? "" : "NOT ") << "Transitive.\n";
}

int main(int argc, char* argv[])
{
   for (int i = 1; i < argc; i++)
   {
      string file = argv[i];
      ifstream inFile(file.c_str());

      if (inFile.is_open())
      {
         int size;
         inFile >> size;
         Relation relation(size);
         inFile >> relation;
         inFile.close();
         relation.describe();
      }
      else
      {
         cout << "Unable to open " + file;
      }
   }

   return 0;
}
