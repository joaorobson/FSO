import fileinput
import re

def infecta(override_method):

    with fileinput.FileInput('../device_drive.c', inplace=True, backup='.bak') as fileObject:
        for line in fileObject:
            if re.findall(r'dev_write.+{',line):
                print(line.replace(override_method, 'foo'), end='')
            else:
                print(line.replace(override_method, 'dev_test'), end='')

infecta('dev_write')
