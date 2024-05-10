#include <algorithm>

#include "ArgParser.hpp"

ArgumentParser::ArgParser::ArgParser(const char* name) {
  name_ = name;
  allowed_typenames_ =
      {typeid(std::string).name(), typeid(CompositeString).name(), typeid(int16_t).name(), typeid(int32_t).name(),
       typeid(int64_t).name(), typeid(uint16_t).name(), typeid(uint32_t).name(), typeid(uint64_t).name(),
       typeid(float).name(), typeid(double).name(), typeid(long double).name(), typeid(bool).name(),
       typeid(char).name()};
  allowed_typenames_for_help_ =
      {"string", "CompositeString", "short", "int", "long long", "unsigned short", "unsigned int", "unsigned long long",
       "float", "double", "long double", "bool", "char"};
  argument_builders_ = {};
  arguments_ = {};
  arguments_by_type_ = {};
  short_to_long_names_ = {};

  for (const std::string& type_name : allowed_typenames_) {
    arguments_by_type_[type_name] = {};
  }

  help_index_ = std::string::npos;
}

ArgumentParser::ArgParser::~ArgParser() {
  for (ArgumentBuilder*& argument_builder : argument_builders_) {
    delete argument_builder;
  }

  for (Argument*& argument : arguments_) {
    delete argument;
  }
}

bool ArgumentParser::ArgParser::Parse(const std::vector<std::string>& args, ConditionalOutput error_output) {
  return Parse_(args, error_output);
}

bool ArgumentParser::ArgParser::Parse(int argc, char** argv, ConditionalOutput error_output) {
  std::vector<std::string> args = std::vector<std::string>(argv, argv + argc);

  return Parse_(args, error_output);
}

bool ArgumentParser::ArgParser::Help() {
  if (help_index_ == std::string::npos) {
    return false;
  }

  return GetValue<bool>(arguments_[help_index_]->GetInfo().long_key);
}

std::string ArgumentParser::ArgParser::HelpDescription() {
  if (help_index_ == std::string::npos) {
    return {};
  }

  std::string help = name_;
  help += "\n";
  help += argument_builders_[help_index_]->GetInfo().description;
  help += "\n\nOPTIONS:\n";

  for (size_t i = 0; i < allowed_typenames_.size(); ++i) {
    std::string type_name = allowed_typenames_[i];
    std::string output_type_name = allowed_typenames_for_help_[i];

    for (const auto& iterator : arguments_by_type_[type_name]) {
      if (iterator.second == help_index_) {
        continue;
      }

      ArgumentBuilder* argument = argument_builders_[iterator.second];
      ArgumentInformation current_info = argument->GetInfo();
      help += current_info.short_key == kBadChar ? "     " : std::string("-") + current_info.short_key + ",  ";
      help += std::string("--") + current_info.long_key;

      if (type_name != typeid(bool).name()) {
        help += "=<" + output_type_name + ">";
      }

      help += ":  ";
      help += current_info.description;

      if (current_info.is_multi_value ||
          current_info.is_positional ||
          (current_info.has_default && type_name == typeid(bool).name() && argument->GetDefaultValue() != "0") ||
          (current_info.has_default && type_name != typeid(bool).name()) ||
          (current_info.minimum_values != 0 && current_info.is_multi_value)) {
        help += " [";
        if (current_info.is_multi_value) {
          help += "repeated, ";
        }

        if (current_info.is_positional) {
          help += "positional, ";
        }

        if (current_info.has_default) {
          help += "default = ";

          if (type_name == typeid(bool).name()) {
            help += (argument->GetDefaultValue() == "0") ? "false" : "true";
          } else {
            help += argument->GetDefaultValue();
          }

          help += ", ";
        }

        if (current_info.minimum_values != 0) {
          help += "min args = " + std::to_string(current_info.minimum_values) + ", ";
        }

        help = help.substr(0, help.size() - 2);
        help += "]";
      }

      help += "\n";
    }
  }

  help += "\n";
  ArgumentBuilder* argument = argument_builders_[help_index_];
  ArgumentInformation current_info = argument->GetInfo();
  help += current_info.short_key == kBadChar ? "     " : std::string("-") + current_info.short_key + ",  ";
  help += std::string("--") + current_info.long_key + ":  " + "Display this help and exit";
  help += "\n";

  return help;
}

ArgumentParser::ConcreteArgumentBuilder<bool>& ArgumentParser::ArgParser::AddHelp(char short_name,
                                                                                  const char* long_name,
                                                                                  const char* description) {
  ConcreteArgumentBuilder<bool>* help_argument_ = &AddArgument<bool>(short_name, long_name, description);
  help_index_ = argument_builders_.size() - 1;
  return *help_argument_;
}

ArgumentParser::ConcreteArgumentBuilder<bool>& ArgumentParser::ArgParser::AddHelp(const char* long_name,
                                                                                  const char* description) {
  return AddHelp(kBadChar, long_name, description);
}

bool ArgumentParser::ArgParser::Parse_(const std::vector<std::string>& args, ConditionalOutput error_output) {
  RefreshArguments();
  std::vector<size_t> used_positions = {0};
  std::vector<std::string> argv = args;

  for (std::string& arg : argv) {
    if (arg[0] == '\'' || arg[0] == '"') {
      arg = arg.substr(1);
    }

    if (arg.back() == '\'' || arg.back() == '"') {
      arg = arg.substr(0, arg.size() - 1);
    }
  }

  argv.emplace_back("--");

  for (size_t position = 1; position < argv.size() && argv[position] != "--"; ++position) {
    if (argv[position][0] == '-') {
      if (argv[position].size() == 1) {
        return false;
      }

      std::vector<std::string> long_keys = GetLongKeys(argv[position]);

      if (long_keys.empty()) {
        DisplayError("Used nonexistent argument: " + argv[position] + "\n", error_output);
        return false;
      }

      for (const std::string& long_key : long_keys) {
        bool was_found = false;

        for (const std::string& type_name : allowed_typenames_) {
          std::map<std::string, size_t>* t_arguments = &arguments_by_type_.at(type_name);

          if (t_arguments->find(long_key) != t_arguments->end()) {
            was_found = true;
            std::vector<size_t> current_used_positions =
                arguments_[t_arguments->at(long_key)]->ValidateArgument(argv, position);
            position = (current_used_positions.empty()) ? position : current_used_positions.back();
            used_positions.insert(std::end(used_positions), std::begin(current_used_positions),
                                  std::end(current_used_positions));
          }

          if (was_found) {
            break;
          }
        }

        if (!was_found) {
          DisplayError("Used nonexistent argument: " + argv[position] + "\n", error_output);
          return false;
        }
      }
    }
  }

  ParsePositionalArguments(argv, used_positions);

  return HandleErrors(error_output);
}

std::vector<std::string> ArgumentParser::ArgParser::GetLongKeys(const std::string& current_argument) {
  std::vector<std::string> long_keys = {current_argument.substr(2)};

  if (long_keys[0].find('=') != std::string::npos) {
    long_keys[0] = long_keys[0].substr(0, long_keys[0].find('='));
  }

  if (current_argument[1] != '-') {
    long_keys.clear();

    for (size_t current_key_index = 1;
         current_key_index < current_argument.size() &&
             short_to_long_names_.find(current_argument[current_key_index]) != short_to_long_names_.end();
         ++current_key_index) {
      long_keys.push_back(short_to_long_names_.at(current_argument[current_key_index]));
    }
  }

  return long_keys;
}

void ArgumentParser::ArgParser::ParsePositionalArguments(const std::vector<std::string>& argv,
                                                         const std::vector<size_t>& used_positions) {
  std::vector<std::string> positional_args = {};
  std::vector<size_t> positional_indices = {};

  for (size_t i = 0; i < arguments_.size(); ++i) {
    if (arguments_[i]->GetInfo().is_positional) {
      positional_indices.push_back(i);
    }
  }

  for (size_t i = 0; i < argv.size(); ++i) {
    if (std::find(std::begin(used_positions), std::end(used_positions), i) ==
        std::end(used_positions)) {
      positional_args.push_back(argv[i]);
    }
  }

  for (size_t position = 0, argument_index = 0;
       position < positional_args.size() &&
           argument_index < positional_indices.size() &&
           positional_args[position] != "--";
       ++position, ++argument_index) {
    std::vector<size_t> current_used_positions =
        arguments_[positional_indices[argument_index]]->ValidateArgument(positional_args, position);
    position = (current_used_positions.empty()) ? position : current_used_positions.back();
  }
}

void ArgumentParser::ArgParser::RefreshArguments() {
  for (Argument* argument : arguments_) {
    argument->ClearStored();
    delete argument;
  }

  arguments_.clear();

  for (ArgumentBuilder* argument_builder : argument_builders_) {
    arguments_.push_back(argument_builder->build());
  }
}

bool ArgumentParser::ArgParser::HandleErrors(ConditionalOutput error_output) {
  std::string error_string;
  bool is_correct = true;

  if (help_index_ != std::string::npos && Help()) {
    return true;
  }

  for (const auto& argument : arguments_) {
    if (!argument->CheckLimit()) {
      error_string += "Not enough values were passed to argument --";
      error_string += argument->GetInfo().long_key;
      error_string += ".\n";
      is_correct = false;
    }

    if (argument->GetValueStatus() == ArgumentParsingStatus::kInvalidArgument) {
      error_string += "An incorrect value was passed to the --";
      error_string += argument->GetInfo().long_key;
      error_string += " argument.\n";
      is_correct = false;
    }
  }

  DisplayError(error_string, error_output);

  return is_correct;
}
