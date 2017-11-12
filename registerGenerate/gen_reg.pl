# generator of expect test file
#   ARGV[0]: test pattern input file name
#   ARGV[1]: output expect file name

$tmp_file = "__temp";

open (INFILE, $ARGV[0]); #"Book2.txt");
open (OUTFILE, ">", $tmp_file); #">timer.txt");
#print OUTFILE "#********************  TEST ", $ARGV[0], " BEGIN  ***********************\n";
$default_desc = "no description";
print "generate svd ";
%desc_start_position = ("D", 5, "P", 5, "R", 5, "F", 5, "E", 3);
%state_str_head = ("P", "\t[",
		           "R", "[",
		           "F", "",
		           "E", "");
%state_str_tail = ("P", "0]\n",
		           "R", "0]\n",
		           "F", "",
		           "E", "");
%state_str_body_start = ("P", "\n\t\t{\n",
			             "R", "\n\t\t\t{\n",
			             "F", "",
			             "E", "");
%state_str_body_end   = ("P", "\t\t},",
			             "R", "\t\t\t},",
			             "F", "",
			             "E", "");

$current_state = IDLE;
@state_array = (IDLE, IDLE, IDLE, IDLE, IDLE, IDLE, IDLE, IDLE); #IDLE, DEVICE, PERIPH, ADDRBLK, INTR, REG, FIELD, ENUMVAL;
$current_pos = 0;


while (! eof) {
    $line =  <INFILE>;
#    print $line;
    $line =~ s/[\n\r]//;
#    print $line;
    @array = split (/[\s]+/, $line);
    $count = @array;
    @subarray = @array[$desc_start_position{$array[0]}..$count-1];
    $list = join(" ", @subarray);

    if ($array[0] eq "D") {
		print OUTFILE "{\n";
		print OUTFILE "\t\"name\":\"", $array[1], "\",";
		print OUTFILE "\t\"version\":\"", $array[2], "\",";
		print OUTFILE "\"groups\": \n";
		$current_state = "D";
    }    
    elsif ($array[0] eq "P") {
		if ($current_state eq "D") {
	    	$current_state = "P";
	    	&push_states();
		}
		elsif ($current_state eq "R") {
		    &pop_states(1);
		}
		elsif ($current_state eq "F") {
		    &pop_states(2);
		}
		elsif ($current_state eq "E") {
		    &pop_states(3);
		}

		&print_periph();
	}
    elsif ($array[0] eq "R") {
		if ($current_state eq "P") {
		    $current_state = "R";
		    &push_states();
		}
		elsif ($current_state eq "R") {
		    &pop_states(0);		    
		}
		elsif ($current_state eq "F") {
		    &pop_states(1);
		}
		elsif ($current_state eq "E") {
		    &pop_states(2);
		}	

		&print_reg();
    }
    elsif ($array[0] eq "F") {
		if ($current_state eq "R") {
		    $current_state = "F";
		    &push_states();
		}
		elsif ($current_state eq "F") {
		    print OUTFILE $state_str_body_end{"F"};
		}
		elsif ($current_state eq "E") {
		    &pop_states(1);
		}
    }
    elsif ($array[0] eq "E") {
		if ($current_state eq "F") {
		    $current_state = "E";
		    &push_states();
		}
		elsif ($current_state eq "E") {
		    print OUTFILE $state_str_body_end{"E"};
		}
    }    
    
#    print ".";
}

&pop_states($current_pos);

print OUTFILE "}\n";
close (INFILE);
close (OUTFILE);

open (INFILE, $tmp_file);
open (OUTFILE, ">", $ARGV[1]);
while (! eof) {
    $line =  <INFILE>;        
	$line =~ s/,0\]/\]/;	
	print OUTFILE $line;
}
close (INFILE);
close (OUTFILE);
print "OK!\n";
unlink($tmp_file);


#/******* sub funtions *********/
sub push_states
{
    $state_array[$current_pos] = $current_state;  # push
    $current_pos ++;
    print OUTFILE  $state_str_head{$current_state};
}

sub pop_states
{
    my($pop_level) = @_;
#    print OUTFILE "d pop level", $pop_level;
    for ($count = 0; $count < $pop_level; $count++) {
	$current_pos --;
	print OUTFILE  $state_str_body_end{$state_array[$current_pos]};
	print OUTFILE  $state_str_tail{$state_array[$current_pos]};
#	print OUTFILE "debug", $current_pos, $state_array[$current_pos];
    }
    if ($current_pos != 0) {
	$current_state = $state_array[$current_pos - 1];
	print OUTFILE  $state_str_body_end{$state_array[$current_pos - 1]};
    }
}

sub print_periph
{
	if($list eq "") {
    	$list = $default_desc;    	
    }
    
    print OUTFILE $state_str_body_start{"P"};
    print OUTFILE "\t\t\t\"name\":\"", $array[1], "\",";
    print OUTFILE "\"desc\":\"", $list, "\",";
    print OUTFILE "\"baseAddr\":\"", $array[4], "\",\n\t\t\t\"registers\":";
}

sub print_reg
{
	if($list eq "") {
    	$list = $default_desc;    	
    }
    
    print OUTFILE $state_str_body_start{"R"};
    print OUTFILE "\t\t\t\t\"name\":\"", $array[1], "\",\n";
    print OUTFILE "\t\t\t\t\"desc\":\"", $list, "\",\n";
    print OUTFILE "\t\t\t\t\"value\":\"\",\n";
    print OUTFILE "\t\t\t\t\"width\":\"", $array[3], "\",\n";
    print OUTFILE "\t\t\t\t\"offset\":\"", $array[2], "\",\n";
    print OUTFILE "\t\t\t\t\"isReadable\":true,\n";
    print OUTFILE "\t\t\t\t\"isWriteable\":true,\n";
    print OUTFILE "\t\t\t\t\"resetValue\":\"\",\n";
    print OUTFILE "\t\t\t\t\"mask\":\"0xFFFFFFFF\",\n";
    print OUTFILE "\t\t\t\t\"bitIndex\":\"0\",\n";
    print OUTFILE "\t\t\t\t\"bitSize\":\"0\"\n";
}
