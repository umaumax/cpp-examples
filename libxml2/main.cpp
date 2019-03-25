#include <iostream>
#include <map>
#include <string>
#include <vector>

#include "libxml/parser.h"

namespace libxml_util {
inline std::string GetName(const xmlNode *node) {
  return std::string(reinterpret_cast<const char *>(node->name));
}
inline std::string GetProp(const xmlNode *node, const std::string &name) {
  const xmlChar *xml_char = reinterpret_cast<const xmlChar *>(name.c_str());
  xmlAttrPtr xml_attr_ptr = xmlHasProp(node, xml_char);
  if (xml_attr_ptr == nullptr) {
    return "";
  }
  xmlChar *xml_prop_char = xmlGetProp(node, xml_char);
  std::string prop(reinterpret_cast<char *>(xml_prop_char));
  xmlFree(xml_prop_char);
  return prop;
}

// NOTE: template arg type: std::function<xmlNodePtr(xmlNodePtr cur)>
// NOTE: e.g. [](xmlNodePtr cur) { return cur; }
template <class... Args>
inline std::vector<xmlNodePtr> CurNextEach(xmlNodePtr cur, Args... args) {
  std::vector<xmlNodePtr> inputs = {cur};
  std::vector<xmlNodePtr> outputs;
  std::vector<std::function<xmlNodePtr(xmlNodePtr cur)>> funcs = {args...};
  for (auto &&f : funcs) {
    for (auto &&cur : inputs) {
      for (; cur != nullptr; cur = cur->next) {
        xmlNodePtr cur_ret = f(cur);
        if (cur_ret != nullptr) {
          outputs.emplace_back(cur_ret);
        };
      }
    }
    // NOTE: for next inputs
    inputs = outputs;
    outputs.clear();
  }
  return inputs;
}

inline xmlNodePtr CurNext(xmlNodePtr cur,
                          std::function<bool(xmlNodePtr cur)> f) {
  for (; cur != nullptr; cur = cur->next) {
    if (f(cur)) {
      break;
    }
  }
  return cur;
}
}  // namespace libxml_util

int main(int argc, char *argv[]) {
  // NOTE: no multi-threaded support
  xmlInitParser();

  xmlDocPtr doc;

  if (argc <= 1) {
    std::cerr << "Usage: " << argv[0] << "[xml filepath]" << std::endl;
    return 0;
  }

  std::string docname(argv[1]);
  doc = xmlParseFile(docname.c_str());
  if (!doc) {
    std::cerr << "[libxml] xmlParseFile error :" << docname << std::endl;
    return 1;
  }

  std::map<std::string, std::string> argument_map;
  xmlNodePtr root_cur = xmlDocGetRootElement(doc);
  xmlNodePtr cur;
  for (cur = root_cur->xmlChildrenNode; cur != nullptr; cur = cur->next) {
    if (libxml_util::GetName(cur) == "objects") {
      break;
    }
  }
  xmlNodePtr objects_cur = cur->xmlChildrenNode;
  // NOTE: chain method version
  {
    libxml_util::CurNextEach(
        objects_cur,
        // NOTE: objects loop
        [](xmlNodePtr cur) {
          return libxml_util::GetName(cur) == "object" &&
                         libxml_util::GetProp(cur, "key") == "test"
                     ? libxml_util::CurNext(
                           cur->xmlChildrenNode,
                           [](xmlNodePtr cur) {
                             return libxml_util::GetName(cur) == "arguments";
                           })
                           ->xmlChildrenNode
                     : nullptr;
        },
        // NOTE: arguments loop
        [&](xmlNodePtr cur) {
          if (libxml_util::GetName(cur) == "argument") {
            std::string argument_key   = libxml_util::GetProp(cur, "name");
            std::string argument_value = libxml_util::GetProp(cur, "value");
            std::cout << "key:" << argument_key << std::endl;
            std::cout << "value:" << argument_value << std::endl;
            argument_map[argument_key] = argument_value;
          }
          return nullptr;
        });
  }
  // NOTE: normal version
  {
    for (; objects_cur != nullptr; objects_cur = objects_cur->next) {
      if (libxml_util::GetName(objects_cur) != "object" ||
          libxml_util::GetProp(objects_cur, "key") != "test") {
        continue;
      }
      xmlNodePtr arguments_cur;
      for (cur = objects_cur->xmlChildrenNode; cur != nullptr;
           cur = cur->next) {
        if (libxml_util::GetName(cur) == "arguments") {
          break;
        }
      }
      arguments_cur = cur->xmlChildrenNode;
      for (; arguments_cur != nullptr; arguments_cur = arguments_cur->next) {
        if (libxml_util::GetName(arguments_cur) != "argument") {
          continue;
        }
        std::string argument_key = libxml_util::GetProp(arguments_cur, "name");
        std::string argument_value =
            libxml_util::GetProp(arguments_cur, "value");
        std::cout << "key:" << argument_key << std::endl;
        std::cout << "value:" << argument_value << std::endl;
        argument_map[argument_key] = argument_value;
      }
    }
  }
  xmlFreeDoc(doc);

  // NOTE: no multi-threaded support
  xmlCleanupParser();
  return 0;
}
