#!/usr/bin/perl
my %input = (X0 => 0, X1 => 1);
my $original;
while(<>) {
    $original = $_;
    if (/might_has_input\(.*->(.*)\)/) {
        # from `might_has_input(self->in)`
        # to `network_not_finished(&Mailbox.n_source[0])`
        s/might_has_input\(.*->(.*)\)/network_not_finished(&Mailbox.n_source\[$input{$1}\])/g;
    }
    if (/ConsumeToken.*/) {
        s/ConsumeToken\(.*->(.*),.*\)/network_consume(&Mailbox.n_source\[$input{$1}\])/g;
    }
    s!actor_Scale!actor_Scale_source!g;
    # todo readtoken
    # print unless $original eq $_;
    print;
}
