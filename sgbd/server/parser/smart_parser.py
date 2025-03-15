from .create_db_parser    import CreateDBParser
from .create_table_parser import CreateTableParser
from .insert_parser       import InsertParser
from .select_parser       import SelectParser

create_db_parser    = CreateDBParser().load()
create_table_parser = CreateTableParser().load()
insert_parser       = InsertParser().load()
select_parser       = SelectParser().load()

parser = (
	create_db_parser    | 
	create_table_parser | 
	insert_parser       | 
	select_parser
)