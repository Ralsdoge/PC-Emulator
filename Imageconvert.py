from PIL import Image
import csv

def ppm_to_csv(ppm_path, csv_path):
    with open(ppm_path, "rb") as f:
        header = f.readline().strip()

        if header not in [b"P3", b"P6"]:
            raise ValueError("Not a valid PPM (P3 or P6 required)")

        # Read header values (skip comments)
        def next_non_comment():
            line = f.readline()
            while line.startswith(b'#'):
                line = f.readline()
            return line

        # width, height
        dims = next_non_comment().split()
        width = int(dims[0])
        height = int(dims[1])

        # max value
        maxval = int(next_non_comment())
        scale = 255.0 / maxval

        import csv
        writer = csv.writer(open(csv_path, "w", newline=""))

        if header == b"P3":
            # ASCII format
            pixels = []
            while len(pixels) < width * height * 3:
                pixels.extend(map(int, next_non_comment().split()))

            idx = 0
            for _ in range(height):
                row = []
                for _ in range(width):
                    r = int(pixels[idx] * scale); idx += 1
                    g = int(pixels[idx] * scale); idx += 1
                    b = int(pixels[idx] * scale); idx += 1
                    row.extend([r, g, b])
                writer.writerow(row)

        else:
            # P6 binary format
            raw = f.read(width * height * 3)
            idx = 0
            for _ in range(height):
                row = []
                for _ in range(width):
                    r = int(raw[idx] * scale); idx += 1
                    g = int(raw[idx] * scale); idx += 1
                    b = int(raw[idx] * scale); idx += 1
                    row.extend([r, g, b])
                writer.writerow(row)

    print("PPM converted correctly →", csv_path)

ppm_to_csv("C:/Users/seaba/Downloads/Telegram Desktop/photo_2025-11-17_20-20-40.ppm","C:/Users/seaba/Downloads/Telegram Desktop/Fixedphoto_2025-11-17_20-20-40.csv")