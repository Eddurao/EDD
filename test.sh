#!/usr/bin/sh
set -ex

# Config
encoder=huffman_coding/encoder
decoder=huffman_coding/decoder
results=huffman.csv
temp_enc=temp.enc
temp_dec=temp.dec
diff_log=diff.log
repeticiones=3

basename() { # https://github.com/dylanaraps/pure-sh-bible?tab=readme-ov-file#get-the-base-name-of-a-file-path
    dir=${1%${1##*[!/]}}
    dir=${dir##*/}
    printf '%s' "${dir:-/}"
}

# Header del .csv
printf "nro_prueba;archivo;tiempo_codificacion;tiempo_decodificacion;size_encoded;size_original\n" > $results

nro_prueba=0
for file in testfiles/*
do
	nro_prueba=$(( $nro_prueba + 1 ))

	# Nombre del archivo, sin la carpeta
	archivo=$(basename $file)

	printf "%s\n" "Testeando $archivo..."

	# Codificacion
	tiempo_codificacion=$($encoder $file $temp_enc $repeticiones)
	
	# Decodificacion
	tiempo_decodificacion=$($decoder $temp_enc $temp_dec $repeticiones)
	
	# Tamaño de los archivos
	size_encoded=$(stat -c %s $temp_enc)
	size_original=$(stat -c %s $file)

	# Comprobación (|| : evita terminal el script si hay un error)
	diff -q $file $temp_dec || :

	# Escribimos los datos en el .csv
	printf "%s\n" "$nro_prueba;$archivo;$tiempo_codificacion;$tiempo_decodificacion;$size_encoded;$size_original" >> $results
done


if [ -f $temp_enc ]
then
	rm $temp_enc
fi

if [ -f $temp_dec ]
then
	rm $temp_dec
fi

