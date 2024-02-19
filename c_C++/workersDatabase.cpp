#ifndef __PROGTEST__
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cctype>
#include <cmath>
#include <cassert>
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <list>
#include <algorithm>
#include <functional>
#include <memory>
using namespace std;
#endif /* __PROGTEST__ */

class CWorker
{
public:
  // Cworker parameters of one worker -> public becuase of the use of class in diffrent class
  string m_name;
  string m_surname;
  string m_email;
  unsigned int m_salary;

  CWorker(const string &name, const string &surname, const string &email, unsigned int salary)
  {
    m_name = name;
    m_surname = surname;
    m_email = email;
    m_salary = salary;
  }
  ~CWorker() {}
  // returns false if the compared Worker is alphabeticaly smaller -> closer to A, compared by surname, then name
  bool operator<(const CWorker &comp_worker) const
  {
    if (m_surname < comp_worker.m_surname)
    {
      return true;
    }
    if (m_surname > comp_worker.m_surname)
    {
      return false;
    }
    if (m_surname == comp_worker.m_surname)
    {
      if (m_name < comp_worker.m_name)
      {
        return true;
      }
    }
    return false;
  }
  // returns false if the compared email is is alphabeticaly smaller -> closer to A
  bool operator<(const string &email) const
  {
    if (m_email < email)
    {
      return true;
    }
    return false;
  }
  // funtion to compare two workers, false if workers have the same name and surname, true otherwaise
  static bool same_worker(const string found_name, const string found_surname, const string tmp_name, const string tmp_surname)
  {
    if (found_name == tmp_name && found_surname == tmp_surname)
    {
      return false;
    }
    return true;
  }
  // funtion to compare two workers, false if workers have the email, true otherwaise
  static bool same_email(const string found_email, const string tmp_email)
  {
    if (found_email == tmp_email)
    {
      return false;
    }
    return true;
  }
};
class CPersonalAgenda
{
public:
  CPersonalAgenda(void) {}
  ~CPersonalAgenda(void)
  {
  }
  // adds a new worker to database, returns false if worker already exists
  bool add(const string &name, const string &surname, const string &email, unsigned int salary)
  {
    // tmp worker to examine and add
    CWorker tmp = CWorker(name, surname, email, salary);
    // looking for palce to store name in vector
    auto it_name = lower_bound(m_databse_name.begin(), m_databse_name.end(), tmp);
    auto it_email = lower_bound(m_databse_email.begin(), m_databse_email.end(), tmp.m_email);
    // control of original name/surname and email
    if (!m_databse_name.empty() && it_name != m_databse_name.end())
    {
      // worker already in database, return false
      if (CWorker::same_worker((*it_name).m_name, (*it_name).m_surname, tmp.m_name, tmp.m_surname) == false)
      {
        return false;
      }
    }
    if (!m_databse_email.empty() && it_email != m_databse_email.end())
    {
      // workers email address isint unique, return false
      if (CWorker::same_email((*it_email).m_email, tmp.m_email) == false)
      {
        return false;
      }
    }
    m_databse_name.insert(it_name, tmp);
    m_databse_email.insert(it_email, tmp);
    return true;
  }
  // delets a worker from database
  bool del(const string &name, const string &surname)
  {
    CWorker tmp = CWorker(name, surname, "", 0);
    auto it_name = lower_bound(m_databse_name.begin(), m_databse_name.end(), tmp);
    // worker not found it_name==end
    if (!m_databse_name.empty() && it_name == m_databse_name.end())
    {
      return false;
    }
    // if name on index is same as name to be deleted -> delete
    tmp.m_email = (*it_name).m_email;
    if (!m_databse_name.empty() && it_name != m_databse_name.end())
    {
      if (CWorker::same_worker((*it_name).m_name, (*it_name).m_surname, tmp.m_name, tmp.m_surname) == false)
      {
        m_databse_name.erase(it_name);
      }
      // worker not found
      else
      {
        return false;
      }
    }
    // deleteing name
    auto it_email = lower_bound(m_databse_email.begin(), m_databse_email.end(), tmp.m_email);
    m_databse_email.erase(it_email);
    return true;
  }
  // delets a worker from database
  bool del(const string &email)
  {
    CWorker tmp = CWorker("", "", email, 0);
    auto it_email = lower_bound(m_databse_email.begin(), m_databse_email.end(), tmp.m_email);
    // worker not found it_name==end
    if (!m_databse_email.empty() && it_email == m_databse_email.end())
    {
      return false;
    }
    tmp.m_name = (*it_email).m_name;
    tmp.m_surname = (*it_email).m_surname;
    // if email on index is same as name to be deleted -> delete
    if (!m_databse_email.empty() && it_email != m_databse_name.end())
    {
      if (CWorker::same_email((*it_email).m_email, tmp.m_email) == false)
      {
        m_databse_email.erase(it_email);
      }
      else
      {
        return false;
      }
    }
    auto it_name = lower_bound(m_databse_name.begin(), m_databse_name.end(), tmp);
    m_databse_name.erase(it_name);
    return true;
  }
  // rename worker
  bool changeName(const string &email, const string &newName, const string &newSurname)
  {
    CWorker tmp = CWorker("", "", email, 0);
    auto it_email = lower_bound(m_databse_email.begin(), m_databse_email.end(), tmp.m_email);
    // worker not found it_name==end
    if (!m_databse_email.empty() && it_email == m_databse_email.end())
    {
      return false;
    }
    if (CWorker::same_email((*it_email).m_email, tmp.m_email) == true)
    {
      return false;
    }
    // check orginality of the new name
    tmp.m_name = newName;
    tmp.m_surname = newSurname;
    auto it_name = lower_bound(m_databse_name.begin(), m_databse_name.end(), tmp);
    if (CWorker::same_worker((*it_name).m_name, (*it_name).m_surname, tmp.m_name, tmp.m_surname) == false)
    {
      return false;
    }
    // rename this worker
    tmp.m_name = (*it_email).m_name;
    tmp.m_surname = (*it_email).m_surname;
    tmp.m_salary = (*it_email).m_salary;
    // found on it_name in the other vector
    it_name = lower_bound(m_databse_name.begin(), m_databse_name.end(), tmp);
    // delete worker from databse - to insert new name
    m_databse_name.erase(it_name);
    m_databse_email.erase(it_email);
    // add back to databse
    tmp.m_name = newName;
    tmp.m_surname = newSurname;
    add(tmp.m_name, tmp.m_surname, tmp.m_email, tmp.m_salary);
    return true;
  }
  // change workers email address
  bool changeEmail(const string &name, const string &surname, const string &newEmail)
  {
    CWorker tmp = CWorker(name, surname, "", 0);
    auto it_name = lower_bound(m_databse_name.begin(), m_databse_name.end(), tmp);
    // worker not found it_name==end
    if (!m_databse_name.empty() && it_name == m_databse_name.end())
    {
      return false;
    }
    if (CWorker::same_worker((*it_name).m_name, (*it_name).m_surname, tmp.m_name, tmp.m_surname) == true)
    {
      return false;
    }
    // check orginalitz of tthe new name
    tmp.m_email = newEmail;
    auto it_email = lower_bound(m_databse_email.begin(), m_databse_email.end(), tmp.m_email);
    if (CWorker::same_email((*it_email).m_email, tmp.m_email) == false)
    {
      return false;
    }
    // rename this worker
    tmp.m_email = (*it_name).m_email;
    tmp.m_salary = (*it_name).m_salary;
    // found on it_name in the other vector
    it_email = lower_bound(m_databse_email.begin(), m_databse_email.end(), tmp.m_email);
    // delete worker from databse - to insert new name
    m_databse_name.erase(it_name);
    m_databse_email.erase(it_email);
    // add back to databse
    tmp.m_email = newEmail;
    add(tmp.m_name, tmp.m_surname, tmp.m_email, tmp.m_salary);
    return true;
  }
  // change salary, if worker not sound, return false
  bool setSalary(const string &name, const string &surname, unsigned int salary)
  {
    CWorker tmp = CWorker(name, surname, "", 0);
    auto it_name = lower_bound(m_databse_name.begin(), m_databse_name.end(), tmp);
    // worker not found it_name==end
    if (!m_databse_name.empty() && it_name == m_databse_name.end())
    {
      return false;
    }
    if (CWorker::same_worker((*it_name).m_name, (*it_name).m_surname, tmp.m_name, tmp.m_surname))
    {
      return false;
    }
    tmp.m_email = (*it_name).m_email;
    (*it_name).m_salary = salary;
    auto it_email = lower_bound(m_databse_email.begin(), m_databse_email.end(), tmp.m_email);
    (*it_email).m_salary = salary;
    return true;
  }
  bool setSalary(const string &email, unsigned int salary)
  {
    CWorker tmp = CWorker("", "", email, 0);
    auto it_email = lower_bound(m_databse_email.begin(), m_databse_email.end(), tmp.m_email);
    // worker not found it_name==end
    if (!m_databse_email.empty() && it_email == m_databse_email.end())
    {
      return false;
    }
    if (CWorker::same_email((*it_email).m_email, tmp.m_email) == true)
    {
      return false;
    }
    tmp.m_name = (*it_email).m_name;
    tmp.m_surname = (*it_email).m_surname;
    (*it_email).m_salary = salary;
    auto it_name = lower_bound(m_databse_name.begin(), m_databse_name.end(), tmp);
    (*it_name).m_salary = salary;
    return true;
  }
  // returns workers salary
  unsigned int getSalary(const string &name, const string &surname) const
  {
    CWorker tmp = CWorker(name, surname, "", 0);
    auto it_name = lower_bound(m_databse_name.begin(), m_databse_name.end(), tmp);
    // worker not found it_name==end
    if (!m_databse_name.empty() && it_name == m_databse_name.end())
    {
      return 0;
    }
    if (CWorker::same_worker((*it_name).m_name, (*it_name).m_surname, tmp.m_name, tmp.m_surname) == true)
    {
      return 0;
    }
    return (*it_name).m_salary;
  }
  unsigned int getSalary(const string &email) const
  {
    CWorker tmp = CWorker("", "", email, 0);
    auto it_email = lower_bound(m_databse_email.begin(), m_databse_email.end(), tmp.m_email);
    // worker not found it_name==end
    if (!m_databse_email.empty() && it_email == m_databse_email.end())
    {
      return 0;
    }
    if (CWorker::same_email((*it_email).m_email, tmp.m_email) == true)
    {
      return 0;
    }

    return (*it_email).m_salary;
  }
  // find out how well the employee is paid in relation to others. The result is the pay position of the specified employee on an imaginary pay scale from worst (lowest) to best (highest).
  bool getRank(const string &name, const string &surname, int &rankMin, int &rankMax) const
  {
    rankMin = 0;
    rankMax = 0;
    CWorker tmp = CWorker(name, surname, "", 0);
    auto it_name = lower_bound(m_databse_name.begin(), m_databse_name.end(), tmp);
    // worker not found it_name==end
    if (!m_databse_name.empty() && it_name == m_databse_name.end())
    {
      return false;
    }
    if (CWorker::same_worker((*it_name).m_name, (*it_name).m_surname, tmp.m_name, tmp.m_surname) == true)
    {
      return false;
    }
    int same_salary = 0;
    for (size_t i = 0; i < m_databse_name.size(); i++)
    {
      if ((*it_name).m_salary > m_databse_name[i].m_salary)
      {
        rankMin++;
      }
      if ((*it_name).m_salary == m_databse_name[i].m_salary)
      {
        same_salary++;
      }
    }
    rankMax = rankMin + same_salary - 1;
    return true;
  }
  bool getRank(const string &email, int &rankMin, int &rankMax) const
  {
    rankMin = 0;
    rankMax = 0;
    CWorker tmp = CWorker("", "", email, 0);
    auto it_email = lower_bound(m_databse_email.begin(), m_databse_email.end(), tmp.m_email);
    // worker not found it_name==end
    if (!m_databse_email.empty() && it_email == m_databse_email.end())
    {
      return 0;
    }
    if (CWorker::same_email((*it_email).m_email, tmp.m_email) == true)
    {
      return 0;
    }
    int same_salary = 0;
    for (size_t i = 0; i < m_databse_email.size(); i++)
    {
      if ((*it_email).m_salary > m_databse_email[i].m_salary)
      {
        rankMin++;
      }
      if ((*it_email).m_salary == m_databse_email[i].m_salary)
      {
        same_salary++;
      }
    }
    rankMax = rankMin + same_salary - 1;
    return true;
  }
  // We want to browse the employees alphabetically (sorted in ascending order by last name, in the case of identical last names by first name).
  // The method returns the first employee in the sorted list, writes his name and surname to the specified output parameters outName/outSurname.
  bool getFirst(string &outName, string &outSurname) const
  {
    if (m_databse_name.empty())
    {
      return false;
    }
    outName = m_databse_name[0].m_name;
    outSurname = m_databse_name[0].m_surname;
    return true;
  }
  // returns the next employee that follows employee name/surname in the sorted list of employees (see getFirst).
  bool getNext(const string &name, const string &surname, string &outName, string &outSurname) const
  {
    CWorker tmp = CWorker(name, surname, "", 0);
    auto it_name = lower_bound(m_databse_name.begin(), m_databse_name.end(), tmp);
    // worker not found it_name==end
    if (!m_databse_name.empty() && it_name == m_databse_name.end())
    {
      return false;
    }
    if (CWorker::same_worker((*it_name).m_name, (*it_name).m_surname, tmp.m_name, tmp.m_surname) == true)
    {
      return false;
    }
    // worker is last
    if (it_name == (m_databse_name.end() - 1))
    {
      return false;
    }
    outName = (*(it_name + 1)).m_name;
    outSurname = (*(it_name + 1)).m_surname;
    return true;
  }

private:
  vector<CWorker> m_databse_name;
  vector<CWorker> m_databse_email;
};

#ifndef __PROGTEST__
int main(void)
{
  string outName, outSurname;
  int lo, hi;

  CPersonalAgenda b1;
  assert(b1.add("John", "Smith", "john", 30000));
  assert(b1.add("John", "Miller", "johnm", 35000));
  assert(b1.add("Peter", "Smith", "peter", 23000));
  assert(b1.getFirst(outName, outSurname) && outName == "John" && outSurname == "Miller");
  assert(b1.getNext("John", "Miller", outName, outSurname) && outName == "John" && outSurname == "Smith");
  assert(b1.getNext("John", "Smith", outName, outSurname) && outName == "Peter" && outSurname == "Smith");
  assert(!b1.getNext("Peter", "Smith", outName, outSurname));
  assert(b1.setSalary("john", 32000));
  assert(b1.getSalary("john") == 32000);
  assert(b1.getSalary("John", "Smith") == 32000);
  assert(b1.getRank("John", "Smith", lo, hi) && lo == 1 && hi == 1);
  assert(b1.getRank("john", lo, hi) && lo == 1 && hi == 1);
  assert(b1.getRank("peter", lo, hi) && lo == 0 && hi == 0);
  assert(b1.getRank("johnm", lo, hi) && lo == 2 && hi == 2);
  assert(b1.setSalary("John", "Smith", 35000));
  assert(b1.getSalary("John", "Smith") == 35000);
  assert(b1.getSalary("john") == 35000);
  assert(b1.getRank("John", "Smith", lo, hi) && lo == 1 && hi == 2);
  assert(b1.getRank("john", lo, hi) && lo == 1 && hi == 2);
  assert(b1.getRank("peter", lo, hi) && lo == 0 && hi == 0);
  assert(b1.getRank("johnm", lo, hi) && lo == 1 && hi == 2);
  assert(b1.changeName("peter", "James", "Bond"));
  assert(b1.getSalary("peter") == 23000);
  assert(b1.getSalary("James", "Bond") == 23000);
  assert(b1.getSalary("Peter", "Smith") == 0);
  assert(b1.getFirst(outName, outSurname) && outName == "James" && outSurname == "Bond");
  assert(b1.getNext("James", "Bond", outName, outSurname) && outName == "John" && outSurname == "Miller");
  assert(b1.getNext("John", "Miller", outName, outSurname) && outName == "John" && outSurname == "Smith");
  assert(!b1.getNext("John", "Smith", outName, outSurname));
  assert(b1.changeEmail("James", "Bond", "james"));
  assert(b1.getSalary("James", "Bond") == 23000);
  assert(b1.getSalary("james") == 23000);
  assert(b1.getSalary("peter") == 0);
  assert(b1.del("james"));
  assert(b1.getRank("john", lo, hi) && lo == 0 && hi == 1);
  assert(b1.del("John", "Miller"));
  assert(b1.getRank("john", lo, hi) && lo == 0 && hi == 0);
  assert(b1.getFirst(outName, outSurname) && outName == "John" && outSurname == "Smith");
  assert(!b1.getNext("John", "Smith", outName, outSurname));
  assert(b1.del("john"));
  assert(!b1.getFirst(outName, outSurname));
  assert(b1.add("John", "Smith", "john", 31000));
  assert(b1.add("john", "Smith", "joHn", 31000));
  assert(b1.add("John", "smith", "jOhn", 31000));

  CPersonalAgenda b2;

  assert(!b2.getFirst(outName, outSurname));
  assert(b2.add("James", "Bond", "james", 70000));
  assert(b2.add("James", "Smith", "james2", 30000));
  assert(b2.add("Peter", "Smith", "peter", 40000));
  assert(!b2.add("James", "Bond", "james3", 60000));
  assert(!b2.add("Peter", "Bond", "peter", 50000));
  assert(!b2.changeName("joe", "Joe", "Black"));
  assert(!b2.changeEmail("Joe", "Black", "joe"));
  assert(!b2.setSalary("Joe", "Black", 90000));
  assert(!b2.setSalary("joe", 90000));
  assert(b2.getSalary("Joe", "Black") == 0);
  assert(b2.getSalary("joe") == 0);
  assert(!b2.getRank("Joe", "Black", lo, hi));
  assert(!b2.getRank("joe", lo, hi));
  assert(!b2.changeName("joe", "Joe", "Black"));
  assert(!b2.changeEmail("Joe", "Black", "joe"));
  assert(!b2.del("Joe", "Black"));
  assert(!b2.del("joe"));
  assert(!b2.changeName("james2", "James", "Bond"));
  assert(!b2.changeEmail("Peter", "Smith", "james"));
  assert(!b2.changeName("peter", "Peter", "Smith"));
  assert(!b2.changeEmail("Peter", "Smith", "peter"));
  assert(b2.del("Peter", "Smith"));
  assert(!b2.changeEmail("Peter", "Smith", "peter2"));
  assert(!b2.setSalary("Peter", "Smith", 35000));
  assert(b2.getSalary("Peter", "Smith") == 0);
  assert(!b2.getRank("Peter", "Smith", lo, hi));
  assert(!b2.changeName("peter", "Peter", "Falcon"));
  assert(!b2.setSalary("peter", 37000));
  assert(b2.getSalary("peter") == 0);
  assert(!b2.getRank("peter", lo, hi));
  assert(!b2.del("Peter", "Smith"));
  assert(!b2.del("peter"));
  assert(b2.add("Peter", "Smith", "peter", 40000));
  assert(b2.getSalary("peter") == 40000);
  return EXIT_SUCCESS;
}
#endif /* __PROGTEST__ */
