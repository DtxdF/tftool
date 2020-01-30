# argprogrammer
> Argprogrammer es una pequeña libreria para el control de los argumentos proporcionados por el usuario.

## Instalación

**Argprogrammer** es fácil de instalar y no requiere de librerias externas

```bash
git clone https://github.com/DtxdF/argprogrammer.git
cd argprogrammer
python3 # o python
```

## Compatibilidad con Python2.x y Python3 :'D

A **argprogrammer** no le importa sí estás usando la versión **2** o **3**, puedes usar todas sus funciones sin problemas

## Tutorial

Te muestro un pequeño tutorial para crear nuestra primera aplicación de forma fácil y sencilla

```python
import argprogrammer

group_optionals = 'opcionales'

parser = argprogrammer.Parser()

parser.add(['-h', '--help'], 'help', 'Mostrar ayuda y sale', group=group_optionals)

parser.add(['-t', '--text'], 'text', 'El texto a mostrar', require=True)

args = parser.parse_args()

text = args.text

print('Texto: %s' % (text))
```

Sí ejecutamos en la terminal: *python o python3 \<script\>.py -h o --help*, veremos ésto:
  
```
  Parámetros principales:
  -----------------------

    -t, --text          El texto a mostrar

  opcionales:
  ----------

    -h, --help          Mostrar ayuda y sale
```

*¿Eso es todo?*

NO, además podemos especificar el tipo de dato y si lo requerimos o no.

```python
import argprogrammer
import requests

group_optionals = 'opcionales'

parser = argprogrammer.Parser()

# Opcionales

parser.add(['-h', '--help'], 'help', 'Mostrar ayuda y sale', group=group_optionals)
parser.add(['-v', '--verbose'], 'verbose', 'Activar modo verboso', action=True, type=bool, group=group_optionals)
parser.add(['-H', '--headers'], 'headers', 'Cambiar los encabezados predeterminados', type=dict, group=group_optionals)

# Requeridos

parser.add(['-u', '--url'], 'url', 'Dirección URL', require=True)

args = parser.parse_args()

url = args.url
verbose = args.verbose
headers = args.headers

if (verbose):

	print('Haciendo una petición a: {}'.format(url))

print('Datos obtenidos: {}'.format(requests.get(url, headers=headers, verify=False).text))
```

En la terminal podrias ejecutar la siguiente sentencia para ver algunos de tus datos cómo el "Agente de Usuaro":

```
python3 <script>.py -u https://www.whatsmyua.info/api/v1/ua -v -H "User-Agent=Hello Friend :D"
```

*Te lo muestra en formato: "**JSON**"*

Podemos hasta incluso hacer que el usuario ingrese sólo algunos datos en un parámetro:

```python
import argprogrammer

group_optionals = 'opcionales'

parser = argprogrammer.Parser()

# Opcionales

parser.add(['-h', '--help'], 'help', 'Mostrar ayuda y sale', group=group_optionals)

# Requeridos

parser.add(['-fruta'], 'fruta', 'Que te gusta comer entre: manzana, durazno, patila', uniqval=['manzana', 'durazno', 'patilla'], require=True)

args = parser.parse_args()

fruta = args.fruta

print('Te gusta comer: {}'.format(fruta))
```

```
python3 <script>.py -fruta patilla
Te gusta comer: patilla
```

En caso de que el usuario no ingrese un valor correcto se muestra lo siguiente:

```
python3 <script>.py -fruta banana
<script>: El valor "banana" no está permitido debido que no es igual a este/estos valor/es "manzana, durazno, patilla"
```

*Se puede personalizar ese mensaje, tranquila/o*

## Tipos de datos

**Argprogrammer** acepta algunos tipos de datos, cómo:

* int
* float
* hex
* bin
* oct
* range
* dict
* str
* list
* tuple

Tal vez pienses, *¿Algunas cosas que mostraste no son funciones?*, Sí, en Python, en argprogrammer No.

## Objetivo

Es sólo una pequeña libreria que codifique para un proyecto personal y que comparto con la comunidad, no creo que se compare con "argparse" pero puede que sirva.

\~ DtxdF
