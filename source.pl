#!/usr/bin/perl
my %input = (ROW => 0, SIGNED => 1);
my $original;
my $name = $ARGV[0];
$name =~ s!.*/(.*)\.c!$1!;
while(<>) {
    $original = $_;
    if (/might_has_input\(.*->([^)]*)\)/) {
        # from `might_has_input(self->in)`
        # to `network_not_finished(&Mailbox.n_source[0])`
        if (exists $input{$1}) {
            s/might_has_input\(.*->([^)]*)\)/network_not_finished(&Mailbox.n_source\[$input{$1}\])/g;
        }
    }
    if (/ReadToken\(.*->(.*),[^)]*\)/) {
        if (exists $input{$1}) {
            s/ReadToken\(.*->(.*),[^)]*\)/network_read(&Mailbox.n_source\[$input{$1}\])/g;
        }
    }
    if (/TestInputPort\(.*->(.*),[^)]*\)/) {
        if (exists $input{$1}) {
            s/TestInputPort\(.*->(.*),[^)]*\)/network_not_finished(&Mailbox.n_source\[$input{$1}\])/g;
        }
    }
    if (/ConsumeToken\(.*->(.*),.*\)/) {
        if (exists $input{$1}) {
            s/ConsumeToken\(.*->(.*),.*\)/network_consume(&Mailbox.n_source\[$input{$1}\])/g;
        }
    }
    s!$name!${name}_source!g;
    # todo readtoken
    # print unless $original eq $_;
    print;
}
