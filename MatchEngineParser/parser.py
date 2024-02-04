import sys
import os
import re


output_dir = ""
headers = []


def is_header_file(filename):
    return filename.endswith(".hpp") or filename.endswith(".h")


def join_path_and_safe_create(path1, path2):
    path = os.path.join(path1, path2)
    if not os.path.exists(path):
        os.makedirs(path)
    if not os.path.isdir(path):
        raise Exception("FATAL: " + path + " is not a directory.")
    return path


class RemoveAnnotationFile:
    """
    去除文件注释 //  /**/  /** */
    """

    def __init__(self, f):
        self.lines = []
        self.line_number = 0

        lines = f.readlines()
        self.need_parse = "REFLECT_CLASS" in ("".join(lines))
        if not self.need_parse:
            return

        in_str = False
        fragment_annotation = False
        for line in lines:
            new_line = ""
            for i, char in enumerate(line):
                if char == '"':
                    in_str = not in_str
                if not in_str:
                    if char == "/" and ((i + 1) < len(line)):
                        if line[i + 1] == '/':
                            break
                        if line[i + 1] == '*':
                            fragment_annotation = True
                if not fragment_annotation:
                    new_line += char
                else:
                    if char == "/" and (i >= 1) and (line[i - 1] == '*'):
                        fragment_annotation = False
            new_line = new_line.strip('\n').rstrip(" ")
            if new_line:
                self.lines.append(new_line + '\n')
                # print(new_line)

    def readline(self):
        if self.line_number == len(self.lines):
            return ""
        self.line_number += 1
        return self.lines[self.line_number - 1]


class FileParser:
    """
    文件解析器, 解析文件并生成代码
    """

    def __init__(self, filename):
        self.filename = filename
        self.match_namespace = re.compile(r"\s*namespace\s+(.+?)\s*{")
        self.match_reflect_class = re.compile(r"\s*REFLECT_CLASS\s*\(.*?\)\s*")
        self.match_reflect_member = re.compile(r"\s*REFLECT_MEMBER\s*\(.*?\)\s*")
        self.match_reflect_function = re.compile(r"\s*REFLECT_FUNCTION\s*\(.*?\)\s*")
        self.match_reflect_member_type = re.compile(r".*?([a-zA-Z_]+?\w*)\s*;")
        self.match_reflect_funtion_type = re.compile(r".*?([a-zA-Z_]+?\w*)\s*\(.*\).*")
        self.match_serializable_class = re.compile(r"\s*REFLECT_CLASS\s*\(.*?SERIALIZABLE_CLASS.*?\)\s*")
        self.match_serializable_class_only = re.compile(r"\s*REFLECT_CLASS\s*\(.*?SERIALIZABLE_CLASS_ONLY.*?\)\s*")
        self.match_serializable_member = re.compile(r"\s*REFLECT_MEMBER\s*\(.*?SERIALIZABLE_MEMBER.*?\)\s*")
        self.match_serializable_member_only = re.compile(r"\s*REFLECT_MEMBER\s*\(.*?SERIALIZABLE_MEMBER_ONLY.*?\)\s*")
        self.match_name = re.compile(r".*?NAME\s*\=\s*\"(.*?)\".*")

        self.register_code_fragment = ""
        self.serialize_code_fragment = ""
    
    def parse(self):
        with open(self.filename, "r") as f:
            raf = RemoveAnnotationFile(f)
            if raf.need_parse:
                self.parse_namespace("", raf, "")
    
    def parse_namespace(self, line, f, prefix):
        count = line.count("{") - line.count("}")

        while True:
            line = f.readline()
            # print(line, count)
            namespaces = self.match_namespace.findall(line)
            # print(namespaces)
            if namespaces:
                self.parse_namespace(line, f, prefix + namespaces[0] + "::")
                continue
            count += line.count("{")
            count -= line.count("}")
            if prefix == "":
                if not line:
                    break
            elif count == 0:
                # print(line, count, "END")
                break
            if bool(self.match_reflect_class.match(line)):
                self.parse_reflect_class(line, f, prefix)
    
    def parse_reflect_class(self, line, f, prefix):
        count = 0
        class_type = ""
        class_name = ""
        member_names = []
        member_aliases = []
        member_serializables = []
        function_names = []
        function_aliases = []

        serializable_class = bool(self.match_serializable_class.match(line))
        serializable_class_only = bool(self.match_serializable_class_only.match(line))

        names = self.match_name.findall(line)
        if names:
            class_name = names[0]
        while True:
            line = f.readline()
            # print(line, count)
            if count == 0:
                class_type = prefix + re.findall(r"[class|struct]\s*([a-zA-Z_]+)\s*{", line)[0]
                if not class_name:
                    class_name = class_type
            if bool(self.match_reflect_class.match(line)):
                self.parse_reflect_class(line, f, class_type + "::")
            count += line.count("{")
            count -= line.count("}")
            if count == 0:
                # print(line, count, "END")
                break
            elif count == 1:
                if bool(self.match_reflect_member.match(line)):
                    serializable_member = False
                    serializable_member_only = False
                    if serializable_class:
                        serializable_member = self.match_serializable_member.match(line)
                        serializable_member_only = self.match_serializable_member_only.match(line)
                    member_serializables.append((serializable_member, serializable_member_only))
                    member_declare = f.readline()
                    count += member_declare.count("{")
                    count -= member_declare.count("}")
                    member_names.append(self.match_reflect_member_type.findall(member_declare)[0])
                    names = self.match_name.findall(line)
                    if names:
                        member_aliases.append(names[0])
                    else:
                        member_aliases.append(member_names[-1])
                elif bool(self.match_reflect_function.match(line)):
                    function_declare = f.readline()
                    count += function_declare.count("{")
                    count -= function_declare.count("}")
                    function_names.append(self.match_reflect_funtion_type.findall(function_declare)[0])
                    names = self.match_name.findall(line)
                    if names:
                        function_aliases.append(names[0])
                    else:
                        function_aliases.append(function_names[-1])
        
        if not serializable_class_only:
            self.register_code_fragment += (f'    ::MatchEngine::ReflectHelper::AddClass<{class_type}>("{class_name}")')
            for i in range(len(member_names)):
                if not (serializable_class and member_serializables[i][1]):
                    self.register_code_fragment += (f'\n        .addMember("{member_aliases[i]}", &{class_type}::{member_names[i]})')
            for i in range(len(function_names)):
                self.register_code_fragment += (f'\n        .addFunction("{function_aliases[i]}", &{class_type}::{function_names[i]})')
            self.register_code_fragment += (';\n\n')
        if serializable_class:
            serializable_members = []
            for i in range(len(member_names)):
                if member_serializables[i][0]:
                    serializable_members.append(member_names[i])
            self.serialize_code_fragment += ('template <>\n')
            self.serialize_code_fragment += (f'struct ::MatchEngine::SerializeTrait<{class_type}> {"{"}\n')
            self.serialize_code_fragment += (f'    static void serialize(SerializeStream &ss, const {class_type} &rhs) {"{"}\n')
            if serializable_members:
                self.serialize_code_fragment += (f'        {" << rhs.".join(["ss"] + serializable_members)};\n')
            self.serialize_code_fragment += (f'    {"}"}\n')
            self.serialize_code_fragment += (f'\n')
            serializable_members.reverse()
            self.serialize_code_fragment += (f'    static void deserialize(DeserializeStream &ds, {class_type} &rhs) {"{"}\n')
            if serializable_members:
                self.serialize_code_fragment += (f'        {" >> rhs.".join(["ds"] + serializable_members)};\n')
            self.serialize_code_fragment += (f'    {"}"}\n')
            self.serialize_code_fragment += ('};\n\n')


# 解析文件并自动生成代码
if __name__ == "__main__":
    index = 0
    for i in range(len(sys.argv)):
        if sys.argv[i].endswith(".py"):
            index = i + 1
            break
    output_dir = sys.argv[index]
    for arg in sys.argv[index + 1:]:
        if os.path.isfile(arg) and is_header_file(arg):
            headers.append(arg)

    header_dir = join_path_and_safe_create(output_dir, "include")
    header_filename = os.path.join(header_dir, "MatchEngine_AutoGenerate.hpp");
    source_dir = join_path_and_safe_create(output_dir, "src")
    source_filename = os.path.join(source_dir, "MatchEngine_AutoGenerate.cpp");
    
    print(header_filename, source_filename)

    write_header = open(header_filename, "w")
    write_source = open(source_filename, "w")

    write_header.write("#pragma once\n\n")
    write_header.write("#include <MatchEngine/core/serialize/specific_serialize_trait.hpp>\n\n")

    write_source.write("#include <MatchEngine/core/reflect/reflect_helper.hpp>\n\n")

    for header_file in headers:
        if header_file != header_filename:
            write_header.write(f'#include "{header_file}"\n')
            write_source.write(f'#include "{header_file}"\n')

    write_header.write("\n")
    write_source.write("\nvoid MatchEngineParser_RegisterReflectProperties() {\n")

    for header in headers:
        parser = FileParser(header)
        parser.parse()
        
        write_header.write(parser.serialize_code_fragment)
        write_source.write(parser.register_code_fragment)

    write_source.write("}\n")

    write_header.flush()
    write_source.flush()

    write_header.close()
    write_source.close()
