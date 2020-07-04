#ifndef FILE_BROWSE_H
#define FILE_BROWSE_H

#ifdef __cplusplus
extern "C" {
#endif

extern int numberOfExportedCharacters;

void getExportedCharacterContents (void);
const char* getExportedCharacterName (int num);
bool getExportedCharacterGender (int num);

#ifdef __cplusplus
}
#endif

#endif // FILE_BROWSE_H
