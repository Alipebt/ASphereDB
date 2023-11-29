#include "version.h"

uint64_t Version::NewFileNumber() { return next_file_number_++; }