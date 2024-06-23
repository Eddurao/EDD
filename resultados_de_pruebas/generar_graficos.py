import pandas as pd
import matplotlib.pyplot as plt

# Leer y procesar datos del CSV
def read_data(file):
    data = pd.read_csv(file, delimiter=';')
    data['compression_ratio'] = data['size_encoded'] / data['size_original']
    data['size_original_MB'] = data['size_original'] / (1024 * 1024)  # Convert size to MB
    return data


huffman_files = {
    'Codigo fuente en C': 'huffman_csources.csv',
    'Secuencias de ADN': 'huffman_dna.csv',
    'Textos en inglés': 'huffman_english.csv',
    'Texto aleatorio': 'huffman_random.csv',
}

lz_files = {
    'Codigo fuente en C': 'LZ_csources.csv',
    'Secuencias de ADN': 'LZ_dna.csv',
    'Textos en inglés': 'LZ_english.csv',
    'Texto aleatorio': 'LZ_random.csv',
}

huffman_data = {key: read_data(file) for key, file in huffman_files.items()}
lz_data = {key: read_data(file) for key, file in lz_files.items()}

# Color de los plots
plot_colors = {
    'huffman_encoding': 'blue',
    'lz_encoding': 'orange',
    'huffman_decoding': 'green',
    'lz_decoding': 'red',
    'huffman_compression': 'purple',
    'lz_compression': 'black'
}

# Funcion de ploteo
def plot_comparison(huffman, lz, file_type, plot_colors):
    fig, axs = plt.subplots(1, 3, figsize=(18, 5))
    
    # Tiempo de codificacion
    axs[0].plot(huffman['size_original_MB'], huffman['tiempo_codificacion'], label='Huffman', marker='o', color=plot_colors['huffman_encoding'])
    axs[0].plot(lz['size_original_MB'], lz['tiempo_codificacion'], label='LZ', marker='o', color=plot_colors['lz_encoding'])
    axs[0].set_title(f'Tiempo de codificación/compresión - {file_type}')
    axs[0].set_xlabel('Tamaño original (MB)')
    axs[0].set_ylabel('Tiempo (ms)')
    axs[0].legend()
    axs[0].grid(True)

    # Tiempo de decodificación
    axs[1].plot(huffman['size_original_MB'], huffman['tiempo_decodificacion'], label='Huffman', marker='o', color=plot_colors['huffman_decoding'])
    axs[1].plot(lz['size_original_MB'], lz['tiempo_decodificacion'], label='LZ', marker='o', color=plot_colors['lz_decoding'])
    axs[1].set_title(f'Tiempo de decodificación/descompresión - {file_type}')
    axs[1].set_xlabel('Tamaño original (MB)')
    axs[1].set_ylabel('Tiempo (ms)')
    axs[1].legend()
    axs[1].grid(True)
    
    # Razon de compresión
    axs[2].plot(huffman['size_original_MB'], huffman['compression_ratio'], label='Huffman', marker='o', color=plot_colors['huffman_compression'])
    axs[2].plot(lz['size_original_MB'], lz['compression_ratio'], label='LZ', marker='o', color=plot_colors['lz_compression'])
    axs[2].set_title(f'Razón de compresión - {file_type}')
    axs[2].set_xlabel('Tamaño original (MB)')
    axs[2].set_ylabel('Razón de compresión')
    axs[2].legend()
    axs[2].grid(True)
    
    plt.tight_layout()
    png_name=file_type.replace(" ", "_")
    plt.savefig(f'graficos/{png_name}_comparación.png')
    plt.close()

# Generamos los plots
for file_type in huffman_files.keys():
    plot_comparison(huffman_data[file_type], lz_data[file_type], file_type, plot_colors)

