# mf

Usage:

copy file to memory
  f2m [ -b base_address ] file_name
extract file from memory
  m2f [ -b base_address]

memory layout, assume base_address is 0x9F00-0000
[9F00 0000] 1K file configuration
[9F00 0400] start of file content

configuration
<filename>\n
<file_size>\n
<MD5 check_sum>\n

