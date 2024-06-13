int main() {
    std::string input_file = "Ruta/del/archivo/entrada"; // Ruta del archivo de entrada
    std::string compressed_file = "Ruta/archivo/comprimido"; // Ruta del archivo comprimido
    std::string decompressed_file = "Ruta/archivo/descomprimido"; // Ruta del archivo descomprimido

    // Verifica si el archivo de entrada existe
    if (!std::ifstream(input_file)) {
        std::cerr << "El archivo de entrada no existe o no se puede acceder: " << input_file << std::endl;
        return 1;
    }

    //Verifica si el archivo de entrada contiene caracteres ASCII o no
    if (!isASCII(input_file)) {
        std::cerr << "El archivo de entrada contiene caracteres que no son ASCII." << std::endl;
        return 1;
    }
    
    //Tiempo de compresión
    auto start_compression = std::chrono::high_resolution_clock::now();
    compress(input_file, compressed_file);
    auto end_compression = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> compression_time = end_compression - start_compression;
    std::cout << "Tiempo de compresion: " << compression_time.count() << " segundos" << std::endl;

    // Verifica existencia del archivo comprimido antes de descomprimir
    if (!std::ifstream(compressed_file)) {
        std::cerr << "El archivo comprimido no existe o no se puede acceder: " << compressed_file << std::endl;
        return 1;
    }
    
    //Tiempo de descompresión
    auto start_decompression = std::chrono::high_resolution_clock::now();
    decompress(compressed_file, decompressed_file);
    auto end_decompression = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> decompression_time = end_decompression - start_decompression;
    std::cout << "Tiempo de descompresion: " << decompression_time.count() << " segundos" << std::endl;


    //Llama a la función para ver el tamaño de los archivos y luego imprime la diferencia de tamaños luego de comprimirlos.
    std::ifstream inputFileStreamSize(input_file, std::ios::binary);
    long input_file_size = sizeArchivo(inputFileStreamSize);
    inputFileStreamSize.close();

    std::ifstream compressedFileStreamSize(compressed_file, std::ios::binary);
    long compressed_file_size = sizeArchivo(compressedFileStreamSize);
    compressedFileStreamSize.close();

    std::cout << "Diferencia de tamaño luego de comprimir " << input_file_size - compressed_file_size << " bytes" << std::endl;
    

    return 0;
}
