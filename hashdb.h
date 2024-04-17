// hashdb.h file
typedef struct hash_struct
{
  int hash; //originally "uint32_t hash;" -BS
  char name[50];
  int salary; //originally "uint32_t salary;" -BS
  struct hash_struct *next;
} hashRecord;