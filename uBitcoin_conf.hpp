// #ifndef __UBITCOIN_CONF_H__
// #define __UBITCOIN_CONF_H__

/* Change this if you want to have other network by default */

#define DEFAULT_NETWORK Mainnet

// /* Change this config file to adjust to your framework */
// #ifndef USE_STDONLY
//   #ifdef ARDUINO
//   #include <Arduino.h>
//   #else
//   #define MBED
//   #include <mbed.h>
//   #endif
// #endif

/* If you don't have a Stream class in your framework you can implement one
 * by yourself and use it to parse transactions and hash on the fly.
 * Arduino and Mbed are using slightly different API, choose one.
 * TODO: describe the interface.
 */