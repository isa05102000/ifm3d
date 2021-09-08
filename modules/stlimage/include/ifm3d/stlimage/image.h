// -*- c++ -*-
/*
 * Copyright 2021-present ifm electronic, gmbh
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef IFM3D_STLIMAGE_IMAGE_H
#define IFM3D_STLIMAGE_IMAGE_H

#include <ifm3d/fg/byte_buffer.h>
#include <cstdint>
#include <memory>
#include <vector>

namespace ifm3d
{
  /**
  @brief The class Image represent a STL conatiner to stored image data  from
  the ifm devices in 2 dimension and supports multiple channel. data is stores
  in sequnetial memory layout and class provides function template to access
  the pixel. Creating an Image object :

  - Use the Create(rows, cols, nchannel, ifm3d::pixel_format ) method or the
  similar Image(nrows, ncols, nchannel, type) constructor.

  For example, FORMAT_8U means a 8-bit array, FORMAT_32F floating-point array,
  and so on.
  @code
      //a 100 x 100 Image of type 8U
      ifm3d::Image image(100,100,1,ifm3d::FORMAT_8U);
      // and now turn image to a 10 x10 3-channel 8-bit matrix.
      // The old content will be deallocated
      image.create(10,10,3,ifm3d::FORMAT_8U);
  @endcode
  As noted in the introduction to this chapter, create() allocates only a new
  array when the memory requiremnt changes for new Image

  - Accessing the pixels
  use at<T>(index) or at<T>(i,j) to access the pixel this return the reference
  to the pixel. A pixel is defined as structure of n-channel values at a given
  index or pixel position in 2D array

  to access a pixel in Image I ( 100,100,1,ifm3d::FORMAT_8U) at  50,50
  position

   @code

     auto pixel = I<uint8_t>(50,50);
     // if working as Index array then

     auto index = 50*100 + 50 ;
     auto pixel = I<uint8_t>(index);

   @endcode

   changing the pixel value can be done as follow :
   writing 100 at pixel postion 50,50

   @code
     I<uint8_t>(50,50) = 100;
     I<uint8_t>(index) = 100;
   @endcode

   to access a pixel in n-channel Image I ( 100,100,3,ifm3d::FORMAT_8U) at
  50,50 position This will be the case accessing the values for 3 channel Image

   as pixel is structure of the values of n-chanel at given position.

   @code

     auto pixel = I<Point3D<uint8_t>>(50,50);

     //now individual channel values can be access with
     value.x, value.y , value.z
   @endcode

   -Processing the whole array
  If you need to process a whole Image, the most efficient way is to
  get the pointer to the row first, and then just use the plain C operator [] :
  @code
     Image I(100,100,1,FORMAT_8U);
      for(int i = 0; i < I.rows; i++)
      {
          const uint8_t* rowi = M.ptr<uint8_t>(i);
          for(int j = 0; j < I.cols; j++)
              {
                //some operation here
              }
      }
  @endcode

  One can aslo use range based for loops with adapter explained
  in ifm3d::IteratorAdapter section
  */
  class Image
  {
  private:
    /* @ brief raw pointer to the data*/
    uint8_t* data_;
    /*@brief number of columns in Image (width)*/
    std::uint32_t cols_;
    /*@brief number of rows in Image (height)*/
    std::uint32_t rows_;
    /*@brief number of channel in Image*/
    std::uint32_t nchannel_;
    /* @brief data format or type*/
    ifm3d::pixel_format data_format_;
    /* @brief number of pixel to store one value of data*/
    std::size_t data_size_in_bytes_;
    /* @brief size of the memory allocated*/
    size_t size_;

    class ImageAllocator;
    std::shared_ptr<ImageAllocator> image_allocator_;

  public:
    /**
      These are various constructors that form a Image.
      default constructor for forming a Image user furher
      needs to call create Method to actually allocates the
      Memory
    */
    Image();
    /*@overload
      @param cols Number of columns in a Image.
      @param rows Number of rows in a Image.
      @param nchannel Number of channels in Image
      @param format value from ifm3d::pixel_format releates to data type
      need to store one value.

      @note This internally calls Create Method to allocates Memory
    */
    Image(const std::uint32_t cols,
          const std::uint32_t rows,
          const std::uint32_t nchannel,
          ifm3d::pixel_format format);

    ~Image() = default;

    // move semantics
    Image(Image&&) = default;
    Image& operator=(Image&&) = default;

    // copy ctor/assignment operator
    Image(const Image&) = default;
    Image& operator=(const Image&) = default;

    /*@brief allocates the memory required for storing the image data
    @param cols Number of columns in a Image.
    @param rows Number of rows in a Image.
    @param nchannel Number of channels in Image
    @param format value from ifm3d::pixel_format releates to data type

    @Note On repeated calling it will deference the old Memory
   */
    void create(const std::uint32_t cols,
                const std::uint32_t rows,
                const std::uint32_t nchannel,
                ifm3d::pixel_format format);

    /** @brief Creates a full copy of the array and the underlying data.
     */
    Image clone() const;

    /* getters*/
    std::uint32_t height() const;
    std::uint32_t width() const;
    std::uint32_t nchannels() const;
    ifm3d::pixel_format dataFormat() const;

    /** @brief returns a pointer to the specified Image row.
        @param row number
     */
    template <typename T = std::uint8_t>
    T* ptr(const std::uint32_t row);

    /**
     @brief Pointer to the Pixel at row,col
     @param row 1st dimension index
     @param col 2nd dimension index
     */
    template <typename T = std::uint8_t>
    T* ptr(const std::uint32_t row, const std::uint32_t col);

    /*@brief access to the pixel for read  and write
      @param Index of the pixel considering image as 1D array
      @return refernce of the value at the index
    */
    template <typename T>
    T& at(const std::uint32_t index);
    /*@overload considering image as 2D
      @param row 1st dimension index
      @param col 2nd dimension index
    */
    template <typename T>
    T& at(const std::uint32_t row, const std::uint32_t col);
    /*@brief set the value where mask value is 1
      @param val  value to be set
      @param mask  Binary mask

      @Note mask size must be same as this
    */
    template <typename T>
    void setTo(const T val, ifm3d::Image& mask);

    /*===========================*/
    /*  Iterators */
    /*===========================*/
    template <typename T>
    struct Iterator
    {
      using iterator_category = std::random_access_iterator_tag;
      using difference_type = std::ptrdiff_t;
      using value_type = T;
      using pointer = T*;
      using reference = T&;

      Iterator(uint8_t* ptr);
      reference operator*() const;
      pointer operator->();
      Iterator& operator++();
      Iterator operator++(std::int32_t);

      friend bool
      operator==(const Iterator& a, const Iterator& b)
      {
        return a.m_ptr == b.m_ptr;
      }

      friend bool
      operator!=(const Iterator& a, const Iterator& b)
      {
        return a.m_ptr != b.m_ptr;
      }

      bool
      operator-(const Iterator& rhs) const noexcept
      {
        // logic here
        return this->m_ptr - rhs.m_ptr; // for example
      }

    private:
      pointer m_ptr;
    };

    /*@brief Return the Iterator pointing to start of data*/
    template <typename T>
    Iterator<T> begin();
    /*@brief Return the Iterator pointing to end of data*/
    template <typename T>
    Iterator<T> end();

  }; // end Image

  /*@brief IteratorAdapter is adapter and can be used in range based loops

  @code
  for (auto value : ifm3d::IteratorAdapter<unsigned short>(image))
  {
      // operation on value
  @endcode
  */
  template <typename T>
  class IteratorAdapter
  {
  private:
    Image& it;

  public:
    IteratorAdapter(Image& it);
    auto begin();
    auto end();
  };

  ////////////////////////////// Image_<Tp>//////////////

  template <typename Tp>
  class Image_ : public Image
  {
  public:
    /*@brief fefault constructor*/
    Image_();

    /* Similar to Image(cols,rows,ifm3d::formatType<Tp>::nchannel,
     * ifm3d::formatType<Tp>::format ) */
    Image_(const std::uint32_t cols, const std::uint32_t rows);

    ~Image_() = default;

    // move semantics
    Image_(Image_<Tp>&&) = default;
    Image_& operator=(Image_<Tp>&&) = default;

    // copy ctor/assignment operator
    Image_(const Image_<Tp>&) = default;
    Image_& operator=(const Image_<Tp>&) = default;

    Image_(const Image&);
    Image_& operator=(const Image&);

    /* Similar to Image::create(cols,rows,ifm3d::formatType<Tp>::nchannel,
     * ifm3d::formatType<Tp>::format ) */
    void create(const std::uint32_t cols, const std::uint32_t rows);

    /** @brief Creates a full copy of the array and the underlying data.
     */
    Image_ clone() const;

    /* getters*/
    std::uint32_t height() const;
    std::uint32_t width() const;
    std::uint32_t nchannels() const;
    ifm3d::pixel_format dataFormat() const;

    /** @brief returns a pointer to the specified Image row.
        @param row number
     */
    Tp* ptr(const std::uint32_t row);

    /**
     @brief Pointer to the Pixel at row,col
     @param row 1st dimension index
     @param col 2nd dimension index
     */
    Tp* ptr(const std::uint32_t row, const std::uint32_t col);

    /*@brief access to the pixel for read  and write
      @param Index of the pixel considering image as 1D array
      @return refernce of the value at the index
    */
    Tp& at(const std::uint32_t index);
    /*@overload considering image as 2D
      @param row 1st dimension index
      @param col 2nd dimension index
    */
    Tp& at(const std::uint32_t row, const std::uint32_t col);
    /*@brief set the value where mask value is 1
      @param val  value to be set
      @param mask  Binary mask

      @Note mask size must be same as this
    */
    void setTo(const Tp val, ifm3d::Image& mask);

    /*@brief Return the Iterator pointing to start of data*/
    Iterator<Tp> begin();
    /*@brief Return the Iterator pointing to end of data*/
    Iterator<Tp> end();

  }; // end Image_<Tp>

  /**
   * @brief Struct for 3D space point
   */
  template <typename T, int n>
  struct point
  {
    T val[n];
    using value_type = T;
  };

  template <typename T>
  using Point3D = struct point<T, 3>;

  template <typename T>
  using Point4D = struct point<T, 4>;

  // user helper types
  using Point3D_16U = Point3D<std::uint16_t>;
  using Point3D_16S = Point3D<std::int16_t>;
  using Point3D_32F = Point3D<float>;

  using Point4D_16U = Point4D<std::uint16_t>;
  using Point4D_16S = Point4D<std::int16_t>;
  using Point4D_32F = Point4D<float>;

} // end: namespace ifm3d

#include <ifm3d/stlimage/detail/image.hpp>
#endif // IFM3D_STLIMAGE_IMAGE_H
