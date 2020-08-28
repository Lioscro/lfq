import gzip
import os
import shutil
import tempfile

from tests.mixins import (
    fastq_factory,
    lfq_decode,
    lfq_encode,
    md5,
    TestMixin,
)


class TestLfq(TestMixin):

    def test_encode_decode(self):
        temp_dir = tempfile.mkdtemp()
        fastq_path = os.path.join(temp_dir, 'test.fastq')
        lfq_path = os.path.join(temp_dir, 'output.lfq')
        header_path = os.path.join(temp_dir, 'output.lfqh')
        qual_path = os.path.join(temp_dir, 'output.lfqq')
        decode_path = os.path.join(temp_dir, 'decoded.fastq')

        try:
            fastq_factory(fastq_path, 10, 10, 100, upper=True)
            lfq_encode(
                self.exe_path, fastq_path, lfq_path, header_path, qual_path
            )
            lfq_decode(
                self.exe_path, lfq_path, decode_path, header_path, qual_path
            )
            self.assertEqual(md5(fastq_path), md5(decode_path))
        finally:
            shutil.rmtree(temp_dir, ignore_errors=True)

    def test_encode_decode_gzip(self):
        temp_dir = tempfile.mkdtemp()
        fastq_path = os.path.join(temp_dir, 'test.fastq')
        gzip_path = f'{fastq_path}.gz'
        lfq_path = os.path.join(temp_dir, 'output.lfq')
        header_path = os.path.join(temp_dir, 'output.lfqh.gz')
        qual_path = os.path.join(temp_dir, 'output.lfqq.gz')
        decode_path = os.path.join(temp_dir, 'decoded.fastq')

        try:
            fastq_factory(fastq_path, 10, 10, 100, upper=True)
            with open(fastq_path, 'rb') as f_in, gzip.open(gzip_path,
                                                           'wb') as f_out:
                shutil.copyfileobj(f_in, f_out)
            lfq_encode(
                self.exe_path, gzip_path, lfq_path, header_path, qual_path
            )
            lfq_decode(
                self.exe_path, lfq_path, decode_path, header_path, qual_path
            )
            self.assertEqual(md5(fastq_path), md5(decode_path))
        finally:
            shutil.rmtree(temp_dir, ignore_errors=True)
