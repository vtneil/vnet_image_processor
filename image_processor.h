#ifndef VNET_IMAGE_PROCESSOR_IMAGE_PROCESSOR_H
#define VNET_IMAGE_PROCESSOR_IMAGE_PROCESSOR_H

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#define GRAYSCALE vnet::COLOR_GRAYSCALE
#define COLOR     vnet::COLOR_RGB

namespace vnet {
    constexpr uint8_t MAX_BIT_DEPTH = 8;

    enum ColorMode {
        COLOR_GRAYSCALE = 0,
        COLOR_RGB,
        COLOR_RGBA
    };

    enum Color {
        R = 0,
        G,
        B,
    };

    template<typename T>
    T max(T a, T b) {
        return a > b ? a : b;
    }

    template<typename T>
    T min(T a, T b) {
        return a < b ? a : b;
    }

    namespace {
        template<ColorMode color_mode, uint8_t bit_depth = 8>
        class Pixel;
    }

    template<ColorMode color_mode, uint8_t bit_depth = 8>
    class Image {
    private:
        typedef Pixel<color_mode, bit_depth> pixel_t;
        size_t _img_width;
        size_t _img_height;
        size_t _buf_len;
        pixel_t **_pixels;
        uint8_t *_buf;

    public:
        Image(size_t width, size_t height) {
            _img_width = width;
            _img_height = height;
            _pixels = new pixel_t *[_img_width]();
            for (size_t i = 0; i < _img_width; ++i)
                _pixels[i] = new pixel_t[_img_height]();
            _buf_len = area() * _pixels[0][0].size() * sizeof(uint8_t);
            _buf = (uint8_t *) malloc(_buf_len);
        }

        Image(size_t width, size_t height, uint8_t *buf) {
            _img_width = width;
            _img_height = height;
            _pixels = new pixel_t *[_img_width]();
            for (size_t i = 0; i < _img_width; ++i) {
                _pixels[i] = new pixel_t[_img_height]();
                for (size_t j = 0; j < _img_height; ++j) {
                    memcpy(_pixels[i][j].data(),
                           buf + _pixels[i][j].size() * (i * _img_height + j),
                           _pixels[i][j].size());
                }
            }
            _buf_len = area() * _pixels[0][0].size() * sizeof(uint8_t);
            _buf = (uint8_t *) malloc(_buf_len);
            memcpy(_buf, buf, buffer_size());
        }

        Image(size_t width, size_t height, uint8_t **pixels) {
            _img_width = width;
            _img_height = height;
            _pixels = new pixel_t *[_img_width]();
            for (size_t i = 0; i < _img_width; ++i) {
                _pixels[i] = new pixel_t[_img_height]();
                for (size_t j = 0; j < _img_height; ++j)
                    _pixels[i][j].set(pixels[i][j]);
            }
            _buf_len = area() * _pixels[0][0].size() * sizeof(uint8_t);
            _buf = (uint8_t *) malloc(_buf_len);
        }

        ~Image() {
            for (size_t i = 0; i < _img_width; ++i)
                delete[] _pixels[i];
            delete[] _pixels;
            free(_buf);
        }

        uint8_t *serialize() {
            for (size_t i = 0; i < _img_width; ++i) {
                for (size_t j = 0; j < _img_height; ++j) {
                    memcpy(_buf + _pixels[i][j].size() * (i * _img_height + j),
                           _pixels[i][j].data(),
                           _pixels[i][j].size());
                }
            }
            return _buf;
        }

        pixel_t &pixel(size_t x, size_t y) {
            return _pixels[x][y];
        }

        size_t width() const {
            return _img_width;
        }

        size_t height() const {
            return _img_height;
        }

        size_t area() const {
            return _img_width * _img_height;
        }

        constexpr size_t buffer_size() const {
            return _buf_len;
        }
    };

    namespace {
        template<ColorMode color_mode, uint8_t bit_depth>
        class Pixel {
        private:
            static constexpr size_t _bit_width = (1 << bit_depth);
            uint8_t *_pixel_data;
            size_t _pixel_size;

        public:
            Pixel() {
                switch (color_mode) {
                    case (COLOR_GRAYSCALE):
                        _pixel_size = 1;
                        break;
                    case (COLOR_RGB):
                        _pixel_size = 3;
                        break;
                    default:
                        _pixel_size = 4;
                        break;
                }
                _pixel_data = (uint8_t *) calloc(_pixel_size, sizeof(uint8_t));
            }

            ~Pixel() {
                free(_pixel_data);
            }

            void set(uint8_t a) {
                switch (color_mode) {
                    case (COLOR_GRAYSCALE):
                        _pixel_data[0] = a;
                        break;
                    default:
                        _pixel_data[Color::R] = a;
                        _pixel_data[Color::G] = a;
                        _pixel_data[Color::B] = a;
                        break;
                }
            }

            void set(uint8_t r, uint8_t g, uint8_t b) {
                switch (color_mode) {
                    case (COLOR_GRAYSCALE):
                        _pixel_data[0] = r;
                        break;
                    default:
                        _pixel_data[Color::R] = r;
                        _pixel_data[Color::G] = g;
                        _pixel_data[Color::B] = b;
                        break;
                }
            }

            uint8_t *data() {
                return _pixel_data;
            }

            size_t size() const {
                return _pixel_size;
            }

            uint8_t depth() const {
                return bit_depth;
            }

            uint8_t intensity() const {
                uint16_t s;
                switch (color_mode) {
                    case (COLOR_GRAYSCALE):
                        s = _pixel_data[0];
                        break;
                    default:
                        s = ((uint16_t) _pixel_data[Color::R] +
                             (uint16_t) _pixel_data[Color::G] +
                             (uint16_t) _pixel_data[Color::B]) / 3;
                        break;
                }
                return (uint8_t) s;
            }

            char repr() const {
                uint8_t _intensity = intensity();
                if (_intensity < _bit_width / 2)
                    return '.';
                else
                    return '#';
            }

            void print() const {
                switch (color_mode) {
                    case (COLOR_GRAYSCALE):
                        printf("(%02x) ", _pixel_data[0]);
                        break;
                    default:
                        printf("(%02x", _pixel_data[Color::R]);
                        printf(",%02x", _pixel_data[Color::G]);
                        printf(",%02x) ", _pixel_data[Color::B]);
                        break;
                }
            }
        };
    }

    template<ColorMode color_mode, uint8_t bit_depth>
    void printi(Image<color_mode, bit_depth> &img) {
        for (size_t i = 0; i < img.height(); ++i) {
            for (size_t j = 0; j < img.width(); ++j) {
                printf("%c ", img.pixel(j, i).repr());
            }
            printf("\n");
        }
    }
}

#endif //VNET_IMAGE_PROCESSOR_IMAGE_PROCESSOR_H
