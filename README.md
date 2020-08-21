# lfq

```
lfq 0.0.1

Usage: lfq <CMD> [arguments] ..

    encode    Encode a set of FASTQ sequences to LFQ
    decode    Decode a LFQ to FASTQ
    index     Generate an index file for fast seeking of LFQ
    view      View one or more sequences in a LFQ

Running lfq <CMD> without arguments prints usage information for <CMD>
```

### encode
```
lfq 0.0.1

Encodes a FASTQ to LFQ format

Usage: lfq encode [options] fastq-file

Positional arguments:
fastq-file      Input FASTQ file. Use a dash '-' to read from stdin.

Options:
-o, --output    (required) File for LFQ output
-h, --header    File for read headers
-q, --qual      File for quality sequences
-i, --index     File for index output
```

### decode
```
lfq 0.0.1

Decodes a LFQ file to FASTQ

Usage: lfq decode [options] lfq-file

Positional arguments:
lfq-file        Input LFQ file.

Options:
-o, --output    File for FASTQ output. Defaults to stdout.
-h, --header    File from which to read headers
-q, --qual      File from which to read quality sequences
```

### index
```
lfq 0.0.1

Builds an index for a LFQ file for fast seeking

Usage: lfq index [options] lfq-file

Positional arguments:
lfq-file        Input LFQ file.

Options:
-o, --output    (required) File for index output.
```

### view
```
lfq 0.0.1

View read(s) in a LFQ file

Usage: lfq view [options] lfq-file

Positional arguments:
lfq-file        Input LFQ file.

Options:
-n, --num       (required) Which read to extract
-i, --index     Index file. Recommended for fast lookup
```
