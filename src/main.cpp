#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <vector>

#include "boost/algorithm/string/predicate.hpp"
#include "reads.hpp"
#include "sequence.hpp"
#include "stream.hpp"

static const char VERSION[] = "0.0.3";

void print_version(const std::string& exe) {
  std::cerr << "lfq " << VERSION << std::endl << std::endl;
}

void print_usage(const std::string& exe) {
  print_version(exe);
  std::cerr << "Usage: " << exe << " <CMD> [arguments] .." << std::endl
            << std::endl
            << "    encode    Encode a set of FASTQ sequences to LFQ"
            << std::endl
            << "    decode    Decode a LFQ to FASTQ" << std::endl
            << "    index     Generate an index file for fast seeking of LFQ"
            << std::endl
            << "    view      View one or more sequences in a LFQ" << std::endl
            << std::endl
            << "Running lfq <CMD> without arguments prints usage information "
               "for <CMD>"
            << std::endl
            << std::endl;
  exit(1);
}

void print_encode_usage(const std::string& exe) {
  print_version(exe);
  std::cerr
      << "Encodes a FASTQ to LFQ format" << std::endl
      << std::endl
      << "Usage: " << exe << " encode [options] fastq-file" << std::endl
      << std::endl
      << "Positional arguments:" << std::endl
      << "fastq-file      Input FASTQ file. Use a dash '-' to read from stdin."
      << std::endl
      << std::endl
      << "Options:" << std::endl
      << "-o, --output    (required) File for LFQ output" << std::endl
      << "-h, --header    File for read headers" << std::endl
      << "-q, --qual      File for quality sequences" << std::endl
      << "-i, --index     File for index output" << std::endl
      << std::endl;
  exit(1);
}

void print_decode_usage(const std::string& exe) {
  print_version(exe);
  std::cerr << "Decodes a LFQ file to FASTQ" << std::endl
            << std::endl
            << "Usage: " << exe << " decode [options] lfq-file" << std::endl
            << std::endl
            << "Positional arguments:" << std::endl
            << "lfq-file        Input LFQ file." << std::endl
            << std::endl
            << "Options:" << std::endl
            << "-o, --output    File for FASTQ output. Defaults to stdout."
            << std::endl
            << "-h, --header    File from which to read headers" << std::endl
            << "-q, --qual      File from which to read quality sequences"
            << std::endl
            << std::endl;
  exit(1);
}

void print_index_usage(const std::string& exe) {
  print_version(exe);
  std::cerr << "Builds an index for a LFQ file for fast seeking" << std::endl
            << std::endl
            << "Usage: " << exe << " index [options] lfq-file" << std::endl
            << std::endl
            << "Positional arguments:" << std::endl
            << "lfq-file        Input LFQ file." << std::endl
            << std::endl
            << "Options:" << std::endl
            << "-o, --output    (required) File for index output." << std::endl
            << std::endl
            << std::endl;
  exit(1);
}

void print_view_usage(const std::string& exe) {
  print_version(exe);
  std::cerr << "View read(s) in a LFQ file" << std::endl
            << std::endl
            << "Usage: " << exe << " view [options] lfq-file" << std::endl
            << std::endl
            << "Positional arguments:" << std::endl
            << "lfq-file        Input LFQ file." << std::endl
            << std::endl
            << "Options:" << std::endl
            << "-n, --num       (required) Which read to extract" << std::endl
            << "-i, --index     Index file. Recommended for fast lookup"
            << std::endl
            << std::endl;
  exit(1);
}

void print_command_usage(const std::string& exe, const std::string& cmd) {
  if (cmd == "encode") {
    print_encode_usage(exe);
  } else if (cmd == "decode") {
    print_decode_usage(exe);
  } else if (cmd == "index") {
    print_index_usage(exe);
  } else if (cmd == "view") {
    print_view_usage(exe);
  } else {
    print_usage(exe);
  }
}

void parse_command(const std::string& exe, const std::string& cmd, int argc,
                   char* argv[], std::vector<std::string>* args,
                   std::map<char, std::string>* opts) {
  for (int i = 0; i < argc; i++) {
    std::string part(argv[i]);

    // Options start with a - and its length is at least 2
    if (part.at(0) == '-' && part.length() > 1) {
      // First non-dash character is used as the key.
      char opt = part.at(1);
      if (opt == '-') {
        // Part starts with two dashes, which can also be used for boolean
        // flags.
        opt = part.at(2);

        // If there aren't any more parts, interpret as a flag.
        if (argc < i + 2) {
          opts->insert(std::pair<char, std::string>(opt, ""));
          continue;
        }
      }

      // Otherwise, there must be at least one more part.
      if (argc < i + 2) {
        print_command_usage(exe, cmd);
      }

      std::string next(argv[i + 1]);
      if (next.at(0) != '-') {
        opts->insert(std::pair<char, std::string>(opt, next));
        i += 1;
      } else {
        opts->insert(std::pair<char, std::string>(opt, ""));
      }
    } else {
      // Otherwise, positional.
      args->push_back(part);
    }
  }
}

void parse_encode(const std::string& exe, const std::vector<std::string>& args,
                  const std::map<char, std::string>& opts) {
  // There must be exactly one argument specifying the input FASTQ.
  if (args.size() != 1) {
    std::cerr << "Error: missing input file" << std::endl;
    print_encode_usage(exe);
  }
  // o option must be provided.
  if (opts.find('o') == opts.end() || opts.at('o').empty()) {
    std::cerr << "Error: missing output file" << std::endl;
    print_encode_usage(exe);
  }

  // Paths as strings
  std::string in_path(args[0]);
  std::string out_path(opts.at('o'));
  std::string index_path(opts.find('i') != opts.end() ? opts.at('i') : "");
  std::string header_path(opts.find('h') != opts.end() ? opts.at('h') : "");
  std::string qual_path(opts.find('q') != opts.end() ? opts.at('q') : "");

  // Actual stream objects for reading/writing
  InStream* in = (in_path == "-") ? new InStream() : new InStream(in_path);
  OutStream* header =
      (!index_path.empty()
           ? new OutStream(index_path, boost::ends_with(index_path, ".gz")
                                           ? StreamType::Gzip
                                           : StreamType::Plain)
           : nullptr);
  OutStream* qual =
      (!qual_path.empty()
           ? new OutStream(qual_path, boost::ends_with(qual_path, ".gz")
                                          ? StreamType::Gzip
                                          : StreamType::Plain)
           : nullptr);
  Reads reads(out_path, ReadsMode::Write);
  Sequence* s = nullptr;
  std::string line;
  // Input FASTQ is a stream from stdin.
  for (size_t i = 0; in->getline(line); i++) {
    switch (i % 4) {
      // Header
      case 0:
        if (header != nullptr) {
          *header << line << std::endl;
        }
        break;
      // Sequence
      case 1:
        s = Sequence::encode(line);
        reads.write_sequence_chunk(*s);
        delete s;
        break;
      // Quality
      case 3:
        if (qual != nullptr) {
          *qual << line << std::endl;
        }
        break;
    }
  }

  if (!index_path.empty()) {
    reads.write_index(index_path);
  }
  delete in;
  delete header;
  delete qual;
}

void parse_decode(const std::string& exe, const std::vector<std::string>& args,
                  const std::map<char, std::string>& opts) {
  // There must be exactly one argument specifying the input LFQ.
  if (args.size() != 1) {
    std::cerr << "Error: missing input file" << std::endl;
    print_encode_usage(exe);
  }
  std::string in_path(args[0]);
  std::string out_path("");
  std::ifstream qual_file;
  std::ifstream header_file;
  if (opts.find('o') != opts.end()) {
    out_path = opts.at('o');
  }
  if (opts.find('h') != opts.end()) {
    header_file.open(opts.at('h'));
  }
  if (opts.find('q') != opts.end()) {
    qual_file.open(opts.at('q'));
  }

  std::ofstream output_file;
  std::ostream& output = (out_path.empty()) ? std::cout : output_file;
  if (!out_path.empty()) {
    output_file.open(out_path, std::ofstream::trunc);
  }

  Reads reads(in_path, ReadsMode::Read);
  Sequence* s = nullptr;
  std::string header;
  std::string seq;
  std::string qual;
  size_t i;
  // Output is to stdout.
  while (true) {
    i = reads.get_read_i();
    s = reads.read_sequence_chunk();
    if (s == nullptr) {
      break;
    }
    seq = s->decode();
    delete s;
    // Header
    if (header_file.is_open()) {
      getline(header_file, header);
      output << header << std::endl;
    } else {
      output << "@" << std::to_string(i) << std::endl;
    }
    // Sequence
    output << seq << std::endl;
    // +
    output << "+" << std::endl;
    // Quality
    if (qual_file.is_open()) {
      getline(qual_file, qual);
      output << qual << std::endl;
    } else {
      output << std::string(seq.length(), '!') << std::endl;
    }
  }

  if (output_file.is_open()) {
    output_file.close();
  }
  if (header_file.is_open()) {
    header_file.close();
  }
  if (qual_file.is_open()) {
    qual_file.close();
  }
}

void parse_index(const std::string& exe, const std::vector<std::string>& args,
                 const std::map<char, std::string>& opts) {
  // There must be exactly one argument specifying the input LFQ.
  if (args.size() != 1) {
    std::cerr << "Error: missing input file" << std::endl;
    print_encode_usage(exe);
  }
  // o option must be provided.
  if (opts.find('o') == opts.end() || opts.at('o').empty()) {
    std::cerr << "Error: missing output file" << std::endl;
    print_encode_usage(exe);
  }
  std::string in_path(args[0]);
  std::string out_path(opts.at('o'));
  Reads reads(in_path, ReadsMode::Read);
  reads.build_index();
  reads.write_index(out_path);
}

void parse_view(const std::string& exe, const std::vector<std::string>& args,
                const std::map<char, std::string>& opts) {
  // There must be exactly one argument specifying the input LFQ.
  if (args.size() != 1) {
    std::cerr << "Error: missing input file" << std::endl;
    print_encode_usage(exe);
  }
  // n option must be provided.
  if (opts.find('n') == opts.end() || opts.at('n').empty()) {
    std::cerr << "Error: missing read number" << std::endl;
    print_encode_usage(exe);
  }
  std::string in_path(args[0]);
  size_t n = stoul(opts.at('n'));
  std::string index_path("");
  if (opts.find('i') != opts.end()) {
    index_path = opts.at('i');
  }
  Reads reads(in_path, ReadsMode::Read);
  if (!index_path.empty()) {
    reads.read_index(index_path);
  }
  reads.seek(n);
  Sequence* s = reads.read_sequence_chunk();
  std::cout << s->decode() << std::endl;
  delete s;
}

int main(int argc, char* argv[]) {
  std::string exe(argv[0]);

  // Generic usage.
  if (argc < 2) {
    print_usage(exe);
  }

  // Command usage.
  std::string cmd(argv[1]);
  if (argc < 3) {
    print_command_usage(exe, cmd);
  }

  // Parse command.
  std::vector<std::string> args;
  std::map<char, std::string> opts;
  parse_command(exe, cmd, argc - 2, argv + 2, &args, &opts);
  if (cmd == "encode") {
    parse_encode(exe, args, opts);
  } else if (cmd == "decode") {
    parse_decode(exe, args, opts);
  } else if (cmd == "index") {
    parse_index(exe, args, opts);
  } else if (cmd == "view") {
    parse_view(exe, args, opts);
  }
}
