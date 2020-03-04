#include <cstdlib>
#include <vector>

/**
  * @brief Rotate elements in a "container"
  * @pre getter and setter are callable objects whichs receives any integer index
  * @param offset Offset of the shift. Positive value rotates to left, and negative value rotates to right
  * @param container_size Number of elements in the container
  * @param get Callable object which returns the element in the n-th position. Parameters: (pos)
  * @param set Callable object which sets the element in the n-th position.    Parameters: (pos, val)
  */
template<typename T, typename Getter, typename Setter>
void RotateElements(int offset, size_t container_size, Getter get, Setter set);


/**
  * @brief Returns positive module of numerator and denominator
  * @pre Denominator isn't zero
  * @param numerator Numerator of module
  * @param denominator Denominator of module
  * @return Module of numerator and denominator. It will be always positive and will be in [0, denominator)
  */
size_t positive_mod(int numerator, size_t denominator);


/**
  * @brief Returns positive module of numerator and denominator
  * @pre Denominator isn't zero
  * @param num Numerator of module
  * @param den Denominator of module
  * @return Module of numerator and denominator. It will be always positive and will be in [0, den)
  */
double positive_mod(double num, double den);

/**
  * @brief Get time from epoch, in seconds
  * @return Returns current time since Epoch, in seconds
  */
double get_current_time();

/**
  * @brief Converts an angle from degrees to radians
  * @param angle Angle in degrees
  * @return Same angle, in radians space
  */
double deg_to_radians(double angle);






/******* IMPLEMENTATION ********/
template<typename T,typename Getter, typename Setter>
void RotateElements(int offset, size_t container_size, Getter get, Setter set) {
    std::vector<T> tmp = std::vector<T>(std::abs(offset));
    int i_offset = 0;
    int increment = (offset < 0 ? -1 : 1);
    if(offset < 0)
        i_offset = 1;
    offset = std::abs(offset);
    for(int i=0; i<offset; ++i)
        tmp[i] = get((i+i_offset)*increment);
    for(int i=0; i<(int)container_size-offset; ++i)
        set((i+i_offset)*increment, get((i+i_offset+offset)*increment));
    for(int i=0; i<offset; ++i)
        set(((int)container_size-offset+i+i_offset)*increment, tmp[i]);
}
