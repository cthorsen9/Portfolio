#include <cstring>
#include <iostream>

using std::cin;
using std::cout;
using std::endl;
using std::string;

bool SubKiller(string const& Wlist, string const& w,
               int pos) {  // catches (most) substring cases
  bool check{true};
  bool no_comma{true};

  if (pos + w.length() < Wlist.length()) {
    if (Wlist.at(pos + w.length()) != ',') check = false;
  }

  else {
    for (int i = 0; i < static_cast<int>(Wlist.length()); i++) {
      if (Wlist.at(i) == ',') {
        no_comma = false;
        break;
      }
    }
    if (no_comma && (w != Wlist)) check = false;
  }

  return check;
}

bool DoubleCheck(string const& whole, string const& w) {
  bool checkr{true}; //checks that whole word and start w match

  for (int i = 1; i < static_cast<int>(w.length()); i++) {
    if ((whole.length() - i < 100) && (w.length() - i < 100)) {
      if ((whole.at(whole.length() - i)) != (w.at(w.length() - i))) {
        checkr = false;
      }
    }
  }
  return checkr;
}

bool Contains(string const& Wlist, string const& whole, string const& w,
              int pos) { //checks if list word and whole word match
  bool is_there{true};
  bool checkr{true};

  if (whole.length() + pos <= Wlist.length() &&
      (whole.length() != Wlist.length())) {
    for (int i = 1; i < static_cast<int>(whole.length()); i++) {
      if (Wlist.at(whole.length() + pos - i) != whole.at(whole.length() - i)) {
        is_there = false;
      }
    }

  } else if ((whole.length() + pos <= Wlist.length()) &&
             (whole.length() == Wlist.length())) {
    for (size_t j = 0; j < Wlist.length(); j++) {
      if (Wlist.at(j) != whole.at(j)) {
        is_there = false;
        break;
      }
    }
  } else
    is_there = false;
  checkr = DoubleCheck(whole, w);

  if (checkr && is_there)
    is_there = true;
  else
    is_there = false;
  return is_there;
}

string GetWord(string const& Wlist, string const& w, int pos) { 
    //gets whole version of word from left comma / start
  int pminus{0};

  string whole_word;

  if (pos != 0) {
    while (pos + pminus > 0) {
      if (Wlist.at(pos + pminus) == ',') {
        whole_word = Wlist.substr(pos + pminus + 1, w.length() - pminus - 1);
        break;
      } else
        pminus--;
    }
    if (pos + pminus == 0)
      whole_word = Wlist.substr(0, w.length() - pminus - 1);
  } else {
    if (Wlist.at(0) != w.at(0)) whole_word = "NOTAWORD";
  }
  if (whole_word == "") whole_word = w;

  for (int u = 0; u < static_cast<int>(whole_word.length()); u++) {
    if (whole_word.at(u) == ',') whole_word = "NOTAWORD";
    if (whole_word.length() == w.length() && (whole_word.at(u) != w.at(u)))
      whole_word = "NOTAWORD";
  }

  return whole_word;
}

bool AtListPosition(string const& Wlist, string const& w,
                    size_t pos) {  // finds if word is at pos in list
  bool still_good = true;
  string whole;
  int com_count{0};
  if (Wlist.at(pos) == w.at(0)) {
    whole = GetWord(Wlist, w, pos);
    still_good = Contains(Wlist, whole, w, pos);

    if (still_good) {
      while ((Wlist.at(com_count) != ',') && com_count + pos < Wlist.length() &&
             Wlist.length() != whole.length()) {
        if (com_count == static_cast<int>(whole.length())) {
          if (Wlist.at(com_count + pos) != ',') still_good = false;
          break;
        } else {
          if ((pos + com_count) <= Wlist.length()) {
            if (Wlist.at(pos + com_count) != whole.at(com_count)) {
              still_good = false;
            }
          } else
            still_good = false;
          com_count++;
        }
      }
    }
  } else
    still_good = false;
  
  if (still_good) still_good = SubKiller(Wlist, whole, pos);
  return still_good;
}

size_t FindInList(string const& Wlist, string const& w,
                  size_t Spos = 0) {  // finds a first instance of word in list,
                                      // optional to start anywhere in list

  size_t this_pos{0};
  this_pos -= 1;

  for (int i = Spos; i < static_cast<int>(Wlist.length()); i++) {
    if (AtListPosition(Wlist, w, i)) {
      this_pos = i;
      break;
    }
  }
  return this_pos;
}

string GetFirstInList(string const& Wlist, string* w1,
                      string* w2) {  // finds which word comes first in list
                                     
  int p1{0};
  int p2{0};
  string first;

  p1 = FindInList(Wlist, *w1);
  p2 = FindInList(Wlist, *w2);

  if (p1 > 10000) p1 = -1;
  if (p2 > 10000) p2 = -1;

  if (p1 == -1 && p2 == -1)
    first = "N/A";
  else if ((p1 < p2 && p1 != -1) || (p1 != -1 && p2 == -1))
    first = *w1;
  else
    first = *w2;

  return first;  
}

size_t CountInList(string const& Wlist, string const& wrd) {
  int word_count{0};

  int prev{0};
  int check{0};
  prev = FindInList(Wlist, wrd, 0);
  if (prev >= 0) word_count++;
  for (int x = 0; x < static_cast<int>(Wlist.length()); x++) {
    
    if (FindInList(Wlist, wrd, x) < 10000) {
      check = FindInList(Wlist, wrd, x);
      if (prev > 10000 || (check > prev)) {
        prev = check;
        word_count++;
      }
    }
  }

  return word_count;
}

int main() {
  string w1;
  string w2;
  string Wlist;

  int count{1};
  while (cin >> Wlist) {
    if (count == 1) {
      w1 = Wlist;
      count++;
    } else if (count == 2) {
      w2 = Wlist;
      count++;
    } else {
      cout << GetFirstInList(Wlist, &w1, &w2) << ' ' << CountInList(Wlist, w1)
           << ' ' << CountInList(Wlist, w2) << endl;
    }
  }
  return 0;
}