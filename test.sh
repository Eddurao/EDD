#!/usr/bin/env sh
set -e

# Config
dataset="rapido"	# Valor default. Opciones: dna , csources , english , random , rapido
results_dir=resultados_de_pruebas
testfiles=testfiles/$dataset/*
results_huffman=$results_dir/huffman_$dataset.csv
results_LZ=$results_dir/LZ_$dataset.csv

# Ejecutables
encoder=huffman_coding/encoder
decoder=huffman_coding/decoder
compresor=compresion_LZ/compressor
decompresor=compresion_LZ/decompressor

temp_enc=temp.enc
temp_dec=temp.dec
diff_log=diff.log
repeticiones=1

if [ -n $1 ]
then
	dataset="$1"
fi

if [ -n $2 ]
then
	repeticiones="$2"
fi

results_huffman=$results_dir/huffman_$dataset.csv
results_LZ=$results_dir/LZ_$dataset.csv

basename() { # https://github.com/dylanaraps/pure-sh-bible?tab=readme-ov-file#get-the-base-name-of-a-file-path
    dir=${1%${1##*[!/]}}
    dir=${dir##*/}
    printf '%s' "${dir:-/}"
}

## Pruebas Huffman encoding y decoding ##
printf "Probando codificación Huffman\n"

# Header del .csv
printf "nro_prueba;archivo;tiempo_codificacion;tiempo_decodificacion;size_encoded;size_original\n" > $results_huffman

nro_prueba=0
for file in $testfiles
do
	nro_prueba=$(( $nro_prueba + 1 ))

	# Nombre del archivo, sin la carpeta
	archivo=$(basename $file)

	printf "Testeando %s...\n" "$archivo"

	# Codificacion
	tiempo_codificacion=$($encoder $file $temp_enc $repeticiones)
	
	# Decodificacion
	tiempo_decodificacion=$($decoder $temp_enc $temp_dec $repeticiones)
	
	# Tamaño de los archivos
	size_encoded=$(stat -c %s $temp_enc)
	size_original=$(stat -c %s $file)

	# Comprobación
	diff_result=$(diff -y --suppress-common-lines $file $temp_dec | wc -l)
	if [ "$diff_result" -gt 1 ]
	then
		printf "Decodificando %s, el archivo decodificado difiere.\n" "$archivo" >> $diff_log
	fi

	# Escribimos los datos en el .csv
	printf "%s\n" "$nro_prueba;$archivo;$tiempo_codificacion;$tiempo_decodificacion;$size_encoded;$size_original" >> $results_huffman
done


## Pruebas Lempel-Ziv compression y decompression ##
printf "Probando compresión Lempel_Ziv\n"

# Header del .csv
printf "nro_prueba;archivo;tiempo_codificacion;tiempo_decodificacion;size_encoded;size_original\n" > $results_LZ

nro_prueba=0
for file in $testfiles
do
	nro_prueba=$(( $nro_prueba + 1 ))

	# Nombre del archivo, sin la carpeta
	archivo=$(basename $file)

	printf "Testeando %s...\n" "$archivo"

	# Codificacion
	tiempo_codificacion=$($compresor $file $temp_enc $repeticiones)
	
	# Decodificacion
	tiempo_decodificacion=$($decompresor $temp_enc $temp_dec $repeticiones)
	
	# Tamaño de los archivos
	size_encoded=$(stat -c %s $temp_enc)
	size_original=$(stat -c %s $file)

	# Comprobación
	diff_result=$(diff -y --suppress-common-lines $file $temp_dec | wc -l)
	if [ "$diff_result" -gt 5 ]
	then
		printf "Descomprimiendo %s, el archivo decodificado difiere:\n" "$archivo" >> $diff_log
	fi

	# Escribimos los datos en el .csv
	printf "%s\n" "$nro_prueba;$archivo;$tiempo_codificacion;$tiempo_decodificacion;$size_encoded;$size_original" >> $results_LZ
done


if [ -f $temp_enc ]
then
	rm $temp_enc
fi

if [ -f $temp_dec ]
then
	rm $temp_dec
fi

