#include <iostream>
#include <string>
#include <cctype>
#include <cstdint>

using namespace std;

// Returns true if a valid address was found, false otherwise.
// On success: outAddress holds the 32-bit value,
// and outPort holds the port number, or -1 if no port was present.
// On failure: outAddress is set to 0 and outPort is set to -1.
bool extractIPv4(const string& str, unsigned long& outAddress, int& outPort)
{
    outAddress = 0;
    outPort = -1;

    for (size_t start = 0; start < str.length(); ++start)
    {
        // An address cannot begin immediately after a period or colon.
        if (start > 0 &&
            (str[start - 1] == '.' || str[start - 1] == ':'))
        {
            continue;
        }

        size_t pos = start;
        uint32_t address = 0;
        int octets[4];
        bool valid = true;

        // Parse exactly four octets.
        for (int octetIndex = 0; octetIndex < 4; ++octetIndex)
        {
            // An octet must contain at least one digit.
            if (pos >= str.length() || !isdigit(static_cast<unsigned char>(str[pos])))
            {
                valid = false;
                break;
            }

            // Leading zero is only allowed for the value 0.
            if (str[pos] == '0' &&
                pos + 1 < str.length() &&
                isdigit(static_cast<unsigned char>(str[pos + 1])))
            {
                valid = false;
                break;
            }

            int value = 0;
            int digitCount = 0;

            // Parse 1-3 digits manually.
            while (pos < str.length() &&
                   isdigit(static_cast<unsigned char>(str[pos])))
            {
                if (digitCount == 3)
                {
                    valid = false;
                    break;
                }

                value = value * 10 + (str[pos] - '0');
                ++digitCount;
                ++pos;
            }

            if (!valid || value > 255)
            {
                valid = false;
                break;
            }

            octets[octetIndex] = value;

            // First three octets must be followed by a period.
            if (octetIndex < 3)
            {
                if (pos >= str.length() || str[pos] != '.')
                {
                    valid = false;
                    break;
                }

                ++pos;
            }
        }

        if (!valid)
        {
            continue;
        }

        // The character immediately after the fourth octet
        // cannot be a period.
        if (pos < str.length() && str[pos] == '.')
        {
            continue;
        }

        // The character immediately before the address cannot be a period/colon.
        // (Already checked above for the starting position.)

        // Build the 32-bit IPv4 value.
        address = (static_cast<uint32_t>(octets[0]) << 24) |
                  (static_cast<uint32_t>(octets[1]) << 16) |
                  (static_cast<uint32_t>(octets[2]) << 8)  |
                  static_cast<uint32_t>(octets[3]);

        int port = -1;

        // Check for optional port.
        if (pos < str.length() && str[pos] == ':')
        {
            ++pos;

            // A colon must be immediately followed by the port.
            if (pos >= str.length() ||
                !isdigit(static_cast<unsigned char>(str[pos])))
            {
                // The colon was part of an attempted address/port,
                // so the entire match is invalid.
                continue;
            }

            // Leading zero is only allowed for port 0.
            if (str[pos] == '0' &&
                pos + 1 < str.length() &&
                isdigit(static_cast<unsigned char>(str[pos + 1])))
            {
                continue;
            }

            int portValue = 0;
            int portDigits = 0;

            // Parse 1-5 digits manually.
            while (pos < str.length() &&
                   isdigit(static_cast<unsigned char>(str[pos])))
            {
                if (portDigits == 5)
                {
                    valid = false;
                    break;
                }

                portValue = portValue * 10 + (str[pos] - '0');
                ++portDigits;
                ++pos;
            }

            if (!valid || portValue > 65535)
            {
                continue;
            }

            port = portValue;

            // A period or colon immediately following the port
            // makes the entire match invalid.
            if (pos < str.length() &&
                (str[pos] == '.' || str[pos] == ':'))
            {
                continue;
            }
        }
        else
        {
            // Without a port, a colon immediately following the
            // fourth octet is already handled above. A second colon
            // cannot be part of a valid address.
            if (pos < str.length() && str[pos] == ':')
            {
                continue;
            }
        }

        // Store the successful result.
        outAddress = static_cast<unsigned long>(address);
        outPort = port;

        return true;
    }

    return false;
}


int main()
{
    string input;

    while (true)
    {
        cout << "Enter a string (or 'END' to quit): ";
        getline(cin, input);

        if (input == "END")
        {
            cout << "Program terminated." << endl;
            break;
        }

        unsigned long address;
        int port;

        if (extractIPv4(input, address, port))
        {
            // Recover the individual octets for displaying the address.
            uint32_t addr = static_cast<uint32_t>(address);

            unsigned int a = (addr >> 24) & 0xFF;
            unsigned int b = (addr >> 16) & 0xFF;
            unsigned int c = (addr >> 8) & 0xFF;
            unsigned int d = addr & 0xFF;

            cout << "Extracted IPv4 address: "
                 << a << "." << b << "." << c << "." << d
                 << " (decimal value: " << address
                 << ", port: ";

            if (port == -1)
            {
                cout << "none";
            }
            else
            {
                cout << port;
            }

            cout << ")" << endl;
        }
        else
        {
            cout << "Invalid input: no valid IPv4 address found" << endl;
        }
    }

    return 0;
}