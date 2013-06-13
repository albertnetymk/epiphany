#!/usr/bin/perl
my %network = (O => 0);
my $original;
my $name = $ARGV[0];
$name =~ s!.*/(.*)\.c!$1!;
while(<>) {
    $original = $_;
    if (/SendToken.*/) {
        s/SendToken\(.*->([^,]*),\s*([^,]*).*\)/network_write(&Mailbox.n_sink\[$network{$1}\], $2)/g;
    }
    s!$name!${name}_sink!g;
    # print unless $original eq $_;
    print;
}
