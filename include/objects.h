#ifndef OBJECTS_H
#define OBJECTS_H
#include <vector>
#include <string>

using namespace std;
/** 
 * \mainpage DNTEncrypt_server documentation
 * 
 * \section intro_sec Introduction
 * The DNTEncrypt server is designed to be extremely simple. It does no processing, 
 * just reading and writing. The server has no need to know anything about the messages,
 * the recipients or senders. Read about the main() function to get started.
 * 
 */

/** 
 * @brief Records a new message into the database
 * 
 * @param IDHASH The identifying hash, this is unique per message and each
 * client is able to independently generate the hash to look for a particular
 * message
 * @param MESSAGE The GnuPG encrypted message
 * @param READERS The identifying hashes of all intended recipients
 * Once all recipients have collected the message, the message is deleted from
 * the database
 * 
 * @return Returns whether or not the recording was successful. Failures should be common 
 * in fact, this indicates that the message hash already exists, and that the client should
 * increment the counter
 */
int database_insert(string IDHASH, string MESSAGE, string READERS);
/** 
 * @brief Retieves a message based on an IDHASH and removes the 
 * READERS hash from the message entry. When all READERS are gone
 * from the entry the entry is deleted
 * 
 * @param IDHASH The identifying hash
 * @param READERS The requesting reader's hash
 * 
 * @return The GnuPG encrypted message
 */
string database_retrieve(string IDHASH, string READERS);

/** 
 * @brief This is the main function. It listens on a particular port 
 * for a message from a client. The client message consists of a command: Read or Write, 
 * the message IDHASH, the encrypted message, and the READERS hashes. A Read message does not
 * carry an encrypted message however, and only has the client's READERS hash. 
 *
 * The read function is database_retrieve(string IDHASH, string READERS).
 * The write function is database_insert(string IDHASH, string MESSAGE, string READERS).
 */
void main();

#endif
