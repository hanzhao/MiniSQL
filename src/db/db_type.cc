#include "db/db_type.h"

#include <ctype.h>
#include <assert.h>
#include <string>
#include <sstream>
#include <iostream>
using namespace std;

#include "db/db_main.h"
#include "base/string.h"

namespace db {
// Create Table class
// create table table_name (
//   attr type(size),
//   attr type(size),
//   ...
//   attr_type(attr),
//   attr_type(attr)
// );
using namespace base;
CreateTable::CreateTable(string command) {
  op_type = TYPE_CREATE_TABLE;
  // Split string into header and definition body.
  size_t bracket_left = command.find('('),
         bracket_right = command.rfind(')');
  if (bracket_left == string::npos ||
      bracket_right == string::npos ||
      bracket_left > bracket_right)
    throw string("Unmatched brackets in table definition.");
  string header = String::Trim(command.substr(0, bracket_left)),
    definition = String::Trim(command.substr(bracket_left + 1,
                                             bracket_right - bracket_left - 1));
  DEBUG << header << endl;
  DEBUG << definition << endl;
  string cur;
  // Parse header to get table_name
  istringstream hin(header);
  hin >> cur;
  assert(cur == "create"); // create
  hin >> cur;
  assert(cur == "table"); // table
  getline(hin, cur, '\0');
  table_name = String::Trim(cur);
  // Check table_name is valid
  if (!isalpha(table_name[0]))
    throw string("Table name should start with a letter.");
  for (size_t i = 0; i < table_name.length(); ++i) {
    if (!isalnum(table_name[i]) && !(table_name[i] == '_'))
      throw string("Table name should contain [A-Za-z0-9_] only.");
  }
  // Parse definitions to get attr_list
  vector<string> definitions = String::Split(definition, ',');
  for (string& s: definitions) {
    s = String::Trim(s);
    // Primary Key definition
    if (s.find("primary key") == 0) {
      string attr_name = String::Trim(String::TakeOffBracket(s.substr(11)));
      bool found = false;
      for (Attribute& attr: attr_list) {
        if (attr.name == attr_name) {
          attr.attribute_type = TYPE_PRIMARY_KEY;
          found = true;
          break;
        }
      }
      if (!found) {
        throw string("Attribute `" + attr_name + "` is not defined.");
      }
    } else {
      Attribute attr;
      attr.attribute_type = TYPE_NONE;
      istringstream in(s);
      in >> attr.name;
      string rest;
      size_t pos;
      getline(in, rest, '\0');
      rest = String::Trim(rest);
      if ((pos = rest.find('(')) != string::npos) {
        string type = String::Trim(rest.substr(0, pos));
        if (type != "char") {
          throw string("Only the size of char type can be specified.");
        }
        attr.type = TYPE_CHAR;
        attr.size = String::ToInt(String::TakeOffBracket(rest.substr(pos)));
      } else {
        if (rest == "int") {
          attr.type = TYPE_INT;
          attr.size = 4;
        } else if (rest == "float") {
          attr.type = TYPE_FLOAT;
          attr.size = 4;
        } else
          throw string("Type `" + rest + "` is not a valid type.");
      }
      DEBUG << "Attr: " << attr.name << " "
                        << attr.type << " "
                        << attr.size << " "
                        << attr.attribute_type << endl;
      attr_list.push_back(attr);
    }
  }
}
int CreateTable::Execute() {
  throw new string("Not implemented");
}
// Drop Table class
DropTable::DropTable(string command) {
  op_type = TYPE_DROP_TABLE;
}
int DropTable::Execute() {
  throw new string("Not implemented");
}
// Create Index class
CreateIndex::CreateIndex(string command) {
  op_type = TYPE_CREATE_INDEX;
}
int CreateIndex::Execute() {
  throw new string("Not implemented");
}
}  // namespace db