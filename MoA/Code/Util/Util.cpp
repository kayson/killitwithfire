/*************************************************************************************************
 *
 * Modeling and animation (TNM079) 2007
 * Code base for lab assignments. Copyright:
 *   Gunnar Johansson (gunnar.johansson@itn.liu.se)
 *   Ken Museth (ken.museth@itn.liu.se)
 *   Michael Bang Nielsen (bang@daimi.au.dk)
 *   Ola Nilsson (ola.nilsson@itn.liu.se)
 *   Andreas Söderström (andreas.soderstrom@itn.liu.se)
 *
 *************************************************************************************************/
#include "Util.h"

std::string& lower(std::string & str) {
  std::transform(str.begin(), str.end(), str.begin(), ::tolower);
  return str;
}

std::string trim(std::string & str) {
  if(str.length() == 0)
    return str;
  std::string::size_type b = str.find_first_not_of(" \t");
  std::string::size_type e = str.find_last_not_of(" \t");
  if(b == std::string::npos) // Contains only whitespace
    return "";
  return std::string(str, b, e - b + 1);
}
