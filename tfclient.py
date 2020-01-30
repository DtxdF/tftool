#!/usr/bin/env python3

# tfclient.py:
#   Es el cliente para poder interactuar con "tftool", hecho en python
#   para mejor comprensión del pequeño protocolo ante los nuevos
#   lectores.

import socket
import os
import sys

from core.python.argprogrammer import argprogrammer

init_error = 0

def check_error(msg):
    global init_error

    if (init_error == 0):
        init_error = 1 # las funciones usan "check_error" para verificar los códigos de estado
                       # con el fin de tener una mayor claridad sobre la operación, pero como
                       # se estarán enviando los datos recibidos a esta función, lo mejor es 
                       # descartarlo cuando ya se haya verificado por primera vez, ya que el
                       # código de estado sólo se envía una vez.

        if (msg == b'1'):
            print("it seems that there is an error on the server or the current operation is not allowed")
            sys.exit(1)

        elif (msg == b'2'):
            print("there was an error opening the remote directory")
            sys.exit(1)

        elif (msg == b'3'):
            print("error opening remote file")
            sys.exit(1)

        elif (msg == b'4'):
            print("the file already exists!")
            sys.exit(1)

        elif (msg == b'5'):
            print("error in the actions!")
            sys.exit(1)

        elif (msg == b'0'):
            print("successful operation!")

def connect(address, port):
    sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM, socket.IPPROTO_TCP)
    sock.connect((address, port))

    return sock

def list_files(fd):
    fd.sendall(b'l:')
    recv = fd.recv(1024)

    while (len(recv) > 0):
        check_error(recv)

        print(recv.decode(), end='')
        recv = fd.recv(1024)

    fd.close()

def upload(fd, file_):
    with open(file_, 'rb') as file_object:
        fd.sendall(('u:%s\0' % (os.path.basename(file_))).encode())
        recv = fd.recv(1)

        check_error(recv)

        fd.sendall(file_object.read())

def download(fd, file_, out):
    fd.sendall(('d:%s' % file_).encode())
    recv = fd.recv(1024)

    check_error(recv)

    with open(out, 'wb') as file_object:
        while (len(recv) > 0):

            file_object.write(recv)
            recv = fd.recv(1024)

    fd.close()

def delete(fd, file_):
    fd.sendall(("D:%s" % (os.path.basename(file_))).encode())
    recv = fd.recv(1)

    if (len(recv) > 0):
        check_error(recv)

    else:
        print("An error has ocurred receiving data...")

    fd.close()

def execute_action(fd, cmd, file_=None, out=None):
    cmd = cmd.lower()

    if (cmd == 'list'):
        list_files(fd)

    elif (cmd == 'upload'):
        upload(fd, file_)

    elif (cmd == 'download'):
        download(fd, file_, out)

    elif (cmd == 'delete'):
        delete(fd, file_)

def main():
    DEFAULT_RPORT = 8043
    DEFAULT_ACTION = ['upload', 'download', 'list', 'delete']

    GROUP_REQUIRED = 'Required'
    GROUP_OPTIONALS = 'Optionals'

    parser = argprogrammer.Parser()

    # Configuramos "argprogrammer" para tenga idioma Inglés

    parser.set_head('''tfclient (Transfer file client): is a util of tftool project to transfer file from a client to a server''')
    parser.set_error_format('The parameter "%s" is not valid')
    parser.set_require_format('The "%s" parameter is necessary to continue the operation.')
    parser.set_uniq_message('The value "%s" is not allowed because it is not equal to "%s"')
    parser.set_type_message('The data type of the value "%s" is not equal to "%s"')

    # Parámetros Opcionales

    parser.add(['-h', '--help'], 'help', 'Show this help', group=GROUP_OPTIONALS)
    parser.add(['-f', '--file'], 'file', 'The file address to download or upload', group=GROUP_OPTIONALS, limit=255)
    parser.add(['-o', '--output'], 'output', 'The filename of output content', group=GROUP_OPTIONALS, limit=255)

    # Parámetros Requeridos

    parser.add(['-a', '--address'], 'address', 'The address to connect', require=True, group=GROUP_REQUIRED)
    parser.add(['-p', '--port'], 'port', 'The port number to connect', type=int, require=True, default=DEFAULT_RPORT,
               group=GROUP_REQUIRED)
    parser.add(['-action'], 'action', 'The action of procedure. Default: [%s]' % (', '.join(DEFAULT_ACTION)), 
               uniqval=DEFAULT_ACTION, require=True, group=GROUP_REQUIRED)

    args = parser.parse_args()

    address = args.address
    port = args.port
    action = args.action
    file_ = args.file
    output = args.output

    try:
        execute_action(connect(address, port), action, file_, output)

    except Exception as Except:
        print('An exception has ocurred: "{}"'.format(Except))

    finally:
        # Comandos cómo "list" pueden no mostrar contenido cuando no haya
        # archivos en el directorio remoto.
        #
        # Aunque si se vuelve molesto, se puede eliminar.

        print("Done.")

if __name__ == '__main__':
    main()
