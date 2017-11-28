import fileinput

def infecta(override_method):

    f = open('../device_drive.c', 'r')
    with open('../device_drive.c', 'r') as fileObject:
        for line in fileObject:
            line = line.replace(override_method, 'dev_test')
            print line

infecta('dev_write')
