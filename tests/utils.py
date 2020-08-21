import hashlib
import subprocess


def md5(path):
    checksum = hashlib.md5()
    with open(path, 'rb') as f:
        while True:
            data = f.read(8192)
            if not data:
                break
            checksum.update(data)
    return checksum.digest()


def lfq_encode(
    exe_path, fastq_path, lfq_path, header_path=None, qual_path=None
):
    command = [exe_path, 'encode', fastq_path]
    command += ['-o', lfq_path]
    if header_path:
        command += ['-h', header_path]
    if qual_path:
        command += ['-q', qual_path]
    return subprocess.run(command, check=True)


def lfq_decode(
    exe_path, lfq_path, fastq_path, header_path=None, qual_path=None
):
    command = [exe_path, 'decode', lfq_path]
    command += ['-o', fastq_path]
    if header_path:
        command += ['-h', header_path]
    if qual_path:
        command += ['-q', qual_path]
    return subprocess.run(command, check=True)
