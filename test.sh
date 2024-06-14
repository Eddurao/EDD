#!/usr/bin/sh
set -e

# Config
encoder=huffman_coding/encoder
results=huffman.csv
temp_file=temp.enc


basename() { # https://github.com/dylanaraps/pure-sh-bible?tab=readme-ov-file#get-the-base-name-of-a-file-path
    dir=${1%${1##*[!/]}}
    dir=${dir##*/}
    printf '%s' "${dir:-/}"
}

# Header del .csv
printf "nro_prueba;archivo;tiempo_codificacion;size_encoded;size_original\n" > $results

nro_prueba=0
for file in testfiles/*
do
	nro_prueba=$(( $nro_prueba + 1 ))

	# Nombre del archivo, sin la carpeta
	archivo=$(basename $file)

	printf "%s\n" "Testeando $archivo..."

	# Codificacion
	tiempo=$($encoder $file $temp_file)
	
	# Tamaño de los archivos
	size_encoded=$(stat -c %s $temp_file)
	size_original=$(stat -c %s $file)

	# Escribimos los datos en el .csv
	printf "%s\n" "$nro_prueba;$archivo;$tiempo;$size_encoded;$size_original" >> $results
done


if [ -f $temp_file ]
then
	rm $temp_file
fi
