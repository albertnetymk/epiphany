#!/usr/bin/perl
my %network = (Y0 => 0, Y1 => 1, Y2 => 2, Y3 => 3);
my $original;
while(<>) {
    $original = $_;
    if (/SendToken.*/) {
        s/SendToken\(.*->([^,]*),\s*([^,]*).*\)/network_write(&Mailbox.n_sink\[$network{$1}\], $2)/g;
    }
    s!actor_Final!actor_Final_sink!g;
    # print unless $original eq $_;
    print;
}
