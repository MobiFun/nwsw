#!/usr/bin/perl
# generate svn version number of each module as a macro
# creates output like #define VERSION_MODID_BSM 6234
# Manu, 02Apr2012

my $now = localtime time;
print "/* generated by $0 on $now */\n";

@array1 = ("BSM","FTH","SCTP","EGTP","TUCL","RRC","S1AP","UEH","DEH","SIH","WIH","LTH","PMH","UDH", "PRM", "CIH1","CIH2","CIH3");
@array2 = ("bsm","fth","sctp","egtpu","tucl","rrc","s1ap","cprm/ueh","deh","sih","wih","lth","pmh","cprm/udh", "prm", "cih","cih","cih");
@array3 = ("MAC","PDCP","RLC","TFU");
@array4 = ("ltemac","lterlcpdcp","ltecl");
my $count = 0;

while($count <= $#array1)
{
  my $result = qx(svnversion -c src/$array2[$count] | cut -d ":" -f2 | tr -d 'M'| tr -d 'S');
  chomp($result);
  print "#define VERSION_MODID_$array1[$count] $result\n";
  $count++;
}
my $count= 0;
while($count <= $#array3)
{
  my $result = qx(svnversion -c lowerarm/$array4[$count] | cut -d ":" -f2 | tr -d 'M'| tr -d 'S');
  chomp($result);
  print "#define VERSION_MODID_$array3[$count] $result\n";
  $count++;
}
