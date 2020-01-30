# -*- coding: UTF-8 -*-

import sys
import collections
import re
import copy
from os.path import basename, splitext

program = splitext(basename(sys.argv[0]))[0]
verbose = lambda string: '{}{}'.format(program if (program == '') else '{}: '.format(program), string)

params = []

default_head = ''
default_footer = ''
default_index = 4
default_footer_row = '-'
default_parameter = 'Parámetros principales'
default_help_key = 'help'
default_error_message = verbose('El parámetro "%s" no es válido')
default_require_message = verbose('El parámetro "%s" es necesario para continuar con la operación')
default_uniq_message = verbose('El valor "%s" no está permitido debido que no es igual a este/estos valor/es "%s"')
default_type_message = verbose('El tipo de dato del valor "%s" no es igual a "%s"')

default_config_help = ''
default_config_type = str
default_config_action = None
default_config_require = False
default_config_value = None
default_config_uniqval = None
default_config_ignorecase = True
default_config_limit = 0

class emptyParams(Exception):

    '''
    Cuando no se agregan parámetros
    '''

class InvalidParams(Exception):

    '''
    Cuando el nombre del parámetro es inválido
    '''

class Parser(object):

    def __init__(self, head=default_head, footer=default_footer, index=default_index, footer_row=default_footer_row, help_key=default_help_key, error_message=default_error_message, require_message=default_require_message, uniq_message=default_uniq_message, type_message=default_type_message, args=None):

        '''
        :head               <str>   : El texto que va ubicado en el encabezado de la ayuda
        :footer             <str>   : El texto que va ubicado en el píe de la ayuda
        :index              <int>   : El indíce de espacios del texto de ayuda (No afecta a «head» ni a «footer»)
        :footer_row         <char>  : El carácter utilizado en las columnas de los nombres de los grupos
        :help_key           <str>   : La clave usada para detectar la ayuda e imprimirla
        :error_message      <str>   : El mensaje usado para invalidar un parámetro. Sintaxis: Debe tener un "%s" y el mensaje adonde se desee
        :require_message    <str>   : El mensaje usado para indicar que un parámetro es requerido. Sintaxis: Igual que "error_message"
        :uniq_message       <str>   : El mensaje usado para inválidar un valor en caso de que se usen valores únicos y debe tener dos "%s" y el mensaje adonde se desee
        :type_message       <str>   : El mensaje usado para inválidar un valor en caso de que no siga el tipo de dato correcto
        :args               <list>  : Los argumentos para usar en la operación. Sí es "None", se usan los argumentos del sistema

        Crear una instancia para interactuar de forma dinámica con los parámetros propuestos
        '''

        self.__groups = {}
        self.__help_param = []
        self.__internal = False

        self.set_head(head)
        self.set_footer(footer)
        self.set_index(index)
        self.set_footer_row(footer_row[:1])
        self.set_help_key(help_key)
        self.set_error_format(error_message)
        self.set_require_format(require_message)
        self.set_uniq_message(uniq_message)
        self.set_type_message(type_message)
        self.set_args(args)

    def __call__(self, *args, **kwargs):

        self.__init__(*args, **kwargs)

    @staticmethod
    def __check_format(message, *args):

        try:

            message % args

        except TypeError:

            raise TypeError('Se necesitan %d para que el formato sea correcto o el tipo de dato usado no es el correcto' % (len(args)))

    def set_args(self, args):

        if not (args == None):

            if not (isinstance(args, list) == True):

                raise TypeError('El tipo de dato de los argumentos no es correcto')

            self.__args = ['']+args
            self.__internal = True

        else:

            self.__args = sys.argv

    def set_type_message(self, format):

        '''
        :type_message       <str>   : El mensaje usado para inválidar un valor en caso de que no siga el tipo de dato correcto

        Ajustar el mensaje para los tipos de datos incorrectos
        '''

        self.__check_format(format, '', '')

        self.__type_message = format

    def set_uniq_message(self, format):

        '''
        :uniq_message       <str>   : El mensaje usado para invalidar un valor en caso de que se usen valores únicos. Debe tener dos "%s" y el mensaje adonde se desee
        
        Ajusta el formato para los parámetros únicos
        '''

        self.__check_format(format, '', '')

        self.__uniq_message = format

    def set_require_format(self, format):

        '''
        :format     <str>: El mensaje usado para indicar que un parámetro es requerido. Sintaxis: Debe tener un "%s" y el mensaje a donde se desee

        Ajusta el formato para los parámetros requeridos
        '''

        self.__check_format(format, '')

        self.__require_message = format

    def set_error_format(self, format):

        '''
        :error_message      <str>   : El mensaje usado para invalidar un parámetro. Sintaxis: Debe tener un "%s" y el mensaje a donde se desee

        Ajusta el formato para los parámetros con errores
        ''' 

        self.__check_format(format, '')

        self.__error_message = format

    def set_index(self, index):

        '''
        :index   <int>: El indíce de espacios del texto de ayuda (No afecta a «head» ni a «footer»)
        
        Ajusta el indíce de espacios en el texto de ayuda
        '''

        self.__index = int(index)

    def set_footer_row(self, footer_row='-'):

        '''
        :footer_row  <char>: El carácter utilizado en los títulos de los nombres de los grupos
        
        Ajusta el carácter de los títulos
        '''

        self.__footer_row = str(footer_row)[:1]

    def set_help_key(self, help_key='help'):

        '''
        :help_key    <str>: La clave usada para detectar la ayuda e imprimirla
        
        Ajusta la clave de ayuda
        '''

        self.__help_key = str(help_key)

    def add(self, param_name, key, help=default_config_help, type=default_config_type, group=default_parameter, action=default_config_action, require=default_config_require, default=default_config_value, uniqval=default_config_uniqval, ignorecase=default_config_ignorecase, limit=default_config_limit):

        '''
        :param_name <list>  : Los parámetros utlizados
        :key        <str>   : La clave o identificador
        :help       <str>   : La ayuda de este parámetro
        :type       <type>  : El tipo de dato a retornar
        :group      <str>   : El nombre del grupo
        :action     <bool>  : En caso de usar un tipo de dato «Booleano» se convierte a «True» o «False», dependiendo si se "llama" en la consola o no
        :default    <str>   : En caso de no especificar un valor en la consola se usa este valor, teniendo en cuentá que debe ser el mismo tipo de dato según el argumento «type»
        :uniqval    <tuple> : Los únicos valores que se pueden usar cómo argumento
        :limit      <int>   : El limite del dato. Usé 0 para que no tenga limitaciones

        Agregá parámetros
        '''
        
        global params

        key = str(key)
        help = str(help)
        group = str(group)
        action = bool(action)
        limit = int(limit)

        if not (isinstance(uniqval, list) == True) and not (uniqval == None):

            raise TypeError('Los valores únicos no están formados en una lista')

        require = bool(require)
        ignorecase = bool(ignorecase)

        try:

            collections.namedtuple('temp', [key])

        except ValueError:

            raise ValueError('El nombre "%s" no es válido' % (key))

        if not (isinstance(param_name, list) == True) and not (isinstance(param_name, tuple)):

            raise TypeError('El tipo de dato no es correcto para los parámetros.') 

        for _ in param_name:

            _exp = _[:3].count('-')

            if (_exp == 3) or (_exp == 0):

                raise InvalidParams('El parámetro "%s" es inválido' % (_))

            if (re.match(r'^(.|..)[0-9]', _)):

                raise InvalidParams('El nombre del parámetro "%s" no debe ser un número' % (_))

        if (self.__groups.get(group.__hash__()) == None):

            self.__groups[group.__hash__()] = {'group':[], 'name':group}
        
        self.__groups[group.__hash__()]['group'].append({'params':param_name, 'action':action, 'key':key, 'type':type, 'help':help, 'default':default, 'require':require, 'uniqval':uniqval, 'ignorecase':ignorecase, 'limit':limit})
        
        params = params+param_name

        if (key == self.__help_key):

            self.__help_param = param_name

    def set_group_head(self, head, group_name=default_parameter):

        '''
        :head       <str>: Añadir un texto en el encabezado del grupo
        :group_name <str>: El nombre del grupo

        :Exception  <KeyError>: Lanzá una excepción, si no existe el nombre del grupo

        Ajusta el texto ubicado en el encabezado del grupo
        '''

        group_name = str(group_name)
        head = str(head)

        try:

            self.__groups[group_name.__hash__()]['head'] = head

        except KeyError:

            raise ValueError('El grupo "%s", no existe ' % (group_name))

    def set_group_footer(self, footer, group_name=default_parameter):

        '''
        :footer     <str>: Añadir un texto en el píe del grupo
        :group_name <str>: El nombre del grupo

        :Exception  <KeyError>: Lanzá una excepción, si no existe el nombre del grupo

        Ajusta el texto ubicado en el píe del grupo
        '''

        group_name = str(group_name)
        footer = str(footer)
        
        try:

            self.__groups[group_name.__hash__()]['footer'] = footer

        except KeyError:

            raise ValueError('El grupo "%s", no existe' % (group_name))

    def set_head(self, head):

        '''
        :head   <str>: El texto que va ubicado en el encabezado de la ayuda
        
        Ajusta el texto ubicado en el encabezado de la ayuda principal
        '''

        self.__head = str(head)

    def set_footer(self, footer):

        '''
        footer  <str>: El texto que va ubicado en el píe de la ayuda

        Ajusta el texto ubicado en el píe de la ayuda principal
        '''

        self.__footer = str(footer)

    @staticmethod
    def __max_length(data):

        _value = data['group']

        _keys = [(', '.join(x['params']), x['help']) for x in _value]
        _result = []
        [_result.append((len(x[0]), x)) for x in _keys]
        _max = max([x[0] for x in _result])
        _max_space = ' '*_max

        return((_max_space, _max, _result))

    def show_format(self):

        '''
        Muestra la ayuda personalizada
        '''

        print(self.__head)
        
        if (len(self.__groups) == 0):

            print('Aún no hay comandos agregados actualmente.')

        else:

            _index = ' '*self.__index
            _footer_row_index = ' '*(self.__index-2) if not (self.__index <= 0) else ''
            _sub_rows_index = ' '*(self.__index-1) if not (self.__index <= 0) else ''
            _max = 0
            _max_space = 0

            for _key, _value in self.__groups.items():

                (a, b, _) = self.__max_length(_value)

                if (b > _max):

                    (_max_space, _max) = (a, b)

            for _key, _value in self.__groups.items():

                _group_name = _value['name']

                (_, _, _result) = self.__max_length(_value)

                print('\n%s:\n%s\n' % (_footer_row_index+_group_name, _footer_row_index+self.__footer_row*len(_group_name))) 
                
                if not (self.__groups[_key].get('head') == None): 
                    
                    print('%s\n' % (_sub_rows_index+self.__groups[_key]['head']))

                for value in _result:  

                    length          = value[0]
                    (_param, _help) = value[1]

                    if (length == _max):

                        print('{}{}{}{}'.format(_index, _param, _max_space, _help))

                    else:

                        _string = '{}{}'.format(_param, _max_space)

                        print('{}{}{}{}'.format(_index, _string[:_max], _max_space, _help))

                if not (self.__groups[_key].get('footer') == None):

                    print()
                    print('%s\n' % (_sub_rows_index+self.__groups[_key]['footer']))

        print(self.__footer)

    def parse_args(self, show_help=True, show_error=True, show_require=True):

        '''
        :show_help      <bool>: Mostrar la ayuda personalizada y sale
        :show_error     <bool>: En caso de una equivocación muestra el parámetro y sale
        :show_require   <bool>: En caso de requerir un parámetro, muestra el faltante y sale

        retorna una colección de los parámetros con sus valor indicados en la consola
        '''

        _result = {}

        show_help = bool(show_help)
        show_error = bool(show_error)
        show_require = bool(show_require)

        if (True in [True for x in self.__help_param if (x in sys.argv)]):

            if (show_help == True):

                self.show_format()
                
                if (self.__internal == False):
                    sys.exit(0)
                else:
                    return

        if (len(self.__groups) == 0):

            raise emptyParams('Aún no se agregaron parámetros')

        if (show_error == True):

            _verify_params = []

            for _arg in self.__args[1:]:

                if not (re.match(r'^(.|..)[0-9]', _arg)):

                    if (_arg[:3].count('-') > 0):

                        _verify_params.append(_arg)

            for _param in _verify_params:

                if not (_param in params):

                    print(self.__error_message % (_param))
                    
                    if (self.__internal == False):
                        sys.exit(1)
                    else:
                        return

        for _group_key, _group_value in self.__groups.items():

            for _a in _group_value['group']:

                key = _a['key']
                default = _a['default']
                bak_default = default
                _type = _a['type']
                action = _a['action']
                require = _a['require']
                ignorecase = _a['ignorecase']
                uniqval = _a['uniqval']
                uniqval = [str(x) for x in uniqval] if not (uniqval == None) else uniqval
                uniqval = [x.lower() for x in uniqval] if (ignorecase == True) and not (uniqval == None) else uniqval
                limit = _a['limit']

                if (_type == bool):

                    _result[key] = not action

                else:

                    _result[key] = default

                if (show_require == True) and (require == True):

                    _required = _a['params']

                    if not (True in [x in self.__args for x in _a['params']]):

                        if (len(_a['params']) >= 3):

                            _param_to_require = ', '.join(_a['params'][:-1])+' o %s' % (_a['params'][-1])

                        else:

                            _param_to_require = ' o '.join(_a['params'])

                        
                        print(self.__require_message % (_param_to_require))
                        if (self.__internal == False):
                            sys.exit(1)
                        else:
                            return

                for _b in _a['params']:

                    try:

                        _index = self.__args.index(_b)

                    except ValueError:

                        continue

                    else:

                        if (_type == bool):

                            _result[key] = action
                            continue

                        try:

                            _value = self.__args[_index+1]

                        except IndexError:

                            _value = bak_default

                        else:

                            if not (uniqval == None):

                                _tmp_value = _value.lower() if (ignorecase == True) else _value

                                if not (_tmp_value in uniqval):

                                    print(self.__uniq_message % (_value, ', '.join(uniqval)))
                                    
                                    if (self.__internal == False):
                                        sys.exit(1)
                                    else:
                                        return

                        finally:

                            if not (_value == None):

                                if (_type == list) or (_type == tuple) or (_type == set):

                                    _value = [x.strip() for x in _value.split(',')]
                                    _value = _value[:limit] if not (limit == 0) else _value
                                    _value = tuple(_value) if (_type == tuple) else _value
                                    _value = set(_value) if (_type == set) else _value

                                elif (_type == dict):

                                    _control = {}

                                    for _ in [x for x in _value.split('&') if not (x.strip() == '')]:

                                        _ = _.split('=')[:2]

                                        try:

                                            (dict_key, dict_value) = _

                                        except ValueError:

                                            if (len(_) == 1):

                                                _control[_[0]] = None

                                        else:

                                            _control[dict_key] = dict_value

                                    _value = _control

                                elif (_type == range):

                                    try:

                                        (range_init, range_end) = [x.strip() for x in _value.split('-')][:2]

                                    except ValueError:

                                        print(self.__type_message % (_value, _type.__name__))
                                        
                                        if (self.__internal == False):
                                            sys.exit(1)
                                        else:
                                            return

                                    else:

                                        try:

                                            range_result = (int(range_init), int(range_end))

                                        except ValueError:

                                            print(self.__type_message % (_value, _type.__name__))
                                            
                                            if (self.__internal == False):
                                                sys.exit(1)
                                            else:
                                                return

                                        else:

                                            if (range_result[0] == 0) and (range_result[1] == 0):

                                                print(self.__type_message % (_value, _type.__name__))
                                                if (self.__internal == False):
                                                    sys.exit(1)
                                                else:
                                                    return

                                            elif (range_result[0] > range_result[1]):

                                                print(self.__type_message % (_value, _type.__name__))
                                                
                                                if (self.__internal == False):
                                                    sys.exit(1)
                                                else:
                                                    return

                                            else:

                                                _value = range_result

                                elif (_type == int) or (_type == float) or (_type == hex) or (_type == bin) or (_type == oct):

                                    try:

                                        _value = _type(int(_value[:limit] if not (limit == 0) else _value))

                                    except (TypeError, ValueError):

                                        print(self.__type_message % (_value, _type.__name__))
                                        
                                        if (self.__internal == False):
                                            sys.exit(1)
                                        else:
                                            return

                                else:

                                    _value = _value[:limit] if not (limit == 0) else _value

                        _result[key] = _value

        # Hago toda esta estupidez para evitar in-compatibilidad con Python2 y Python3

        _parse_keys = []
        _parse_values = []

        for _parse_key, _parse_value in _result.items():

            _parse_keys.append(_parse_key)
            _parse_values.append(_parse_value)

        _elements = collections.namedtuple('args', _parse_keys)
        _result = _elements(*_parse_values)

        return(_result)
