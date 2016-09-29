# generates strings for enum values: e_BsErrorValue
 
$h_file=shift; # bs_err.h
open(IN,  "<",  $h_file)  or die "Can't open $h_file: $!";
# search for enums like BS_ERR_SPAWN, and generate a string for them

my $now = localtime time;
print "/* generated by $0 on $now */\n";
print "#include \"all_cmn.h\"\n";


print "typedef struct s_BsErrorName {\n  BsErrorValue error;\n  const String100 name;\n} BsErrorName;\n\n";
print "const BsErrorName c_bsErrNameTab[] =\n";
print "{\n";
printf "  {BS_NO_ERROR, %50s},\n", "\"BS_NO_ERROR\"";

while (<IN>)
{
  if (/(BS_ERR_.*),/)
  {
    $msgId = $1;
    $msgId =~ s/[\s]//g;
    printf "  {$msgId, %50s},\n", "\"$msgId\"";
  }
}

printf "  {0, %30s}\n", "\"\"";
print "};\n";
close IN;

print "\n";
print "const UINT8* errorCodeStr( IN BsErrorValue enumNum)\n";
print "{\n";
print "  if( (enumNum < BS_NO_ERROR) || (enumNum > BS_ERR_UNSPECIFIED) )\n";
print "  {\n";
print "    return \"name-not-found\";\n";
print "  }\n";
print "  else\n";
print "  {\n";
print "    return &c_bsErrNameTab[enumNum - BS_NO_ERROR ].name[0];\n";
print "  }\n";
print "}\n\n";
