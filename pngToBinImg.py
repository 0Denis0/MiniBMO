import os
from PIL import Image
import numpy as np

def convert_png_to_bin(png_path, bin_path, width, height):
    # Open the PNG image
    image = Image.open(png_path).convert('1')  # Convert to 1-bit monochrome

    # Resize the image to fit the display
    # image = image.resize((width, height), Image.LANCZOS)

    # Convert the image to a numpy array
    image_array = np.array(image)

    # Create a list to store the byte array
    byte_array = []

    # Convert the image array to byte array
    for y in range(height):
        for x in range(0, width, 8):
            byte = 0
            for bit in range(8):
                if x + bit < width:
                    byte |= (image_array[y, x + bit] == 0) << (7 - bit)
            byte_array.append(byte)

    # Write the byte array to the binary file
    with open(bin_path, 'wb') as bin_file:
        bin_file.write(bytearray(byte_array))
    print(f"Converted {png_path} to {bin_path}")

def main():
    # Parameters
    png_dir = 'assets/pngs/animationFrames'
    bin_dir = 'assets/bins/bitmaps'
    width = 128
    height = 64

    # Ensure the bin directory exists
    os.makedirs(bin_dir, exist_ok=True)

    # Process each PNG file in the directory
    for filename in os.listdir(png_dir):
        if filename.endswith('.png'):
            png_path = os.path.join(png_dir, filename)
            bin_path = os.path.join(bin_dir, os.path.splitext(filename)[0] + '.bin')
            convert_png_to_bin(png_path, bin_path, width, height)

if __name__ == '__main__':
    main()
