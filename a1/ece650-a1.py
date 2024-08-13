
import sys
import math
import re

class Point(object):
    def __init__ (self, x, y):
        self.x = float(x)
        self.y = float(y)
    def __str__ (self):
        return '(' + str(self.x) + ',' + str(self.y) + ')'
    
    def __eq__(self, other):
        if isinstance(other, Point):
            return self.x == other.x and self.y==other.y
        return False

    def __hash__(self):
        return hash(self.x + self.y)
    
    
def calculate_distance(point1, point2):
    return math.sqrt((point2.x - point1.x)**2 + (point2.y - point1.y)**2)


class Line(object):
    def __init__ (self, src, dst):
        self.src = src
        self.dst = dst

    def __str__(self):
        return str(self.src) + '-->' + str(self.dst)


class Street(object):
    def __init__(self, name, line_List):
        self.name=name
        self.line_List=line_List

    def __str__(self):
        return str(self.name) + '-->' + str(self.line_List)
    

def get_Intersection(street_list):

    list_intersection=[]
    unique_set_intersection=set()
  
    for street in street_list:
        for line in street.line_List:
            for street_2 in street_list:
                for line_2 in street_2.line_List:
                    if street != street_2:
                        
                        list_intersection.append(intersect(line,line_2))
  
    for i in list_intersection:
        if i != None:
            unique_set_intersection.add(i)
        
    return unique_set_intersection
    
   
def on_segment(p, q, r, tolerance=1e-10):
    min_x = min(p.x, r.x) - tolerance
    max_x = max(p.x, r.x) + tolerance
    min_y = min(p.y, r.y) - tolerance
    max_y = max(p.y, r.y) + tolerance

    return (
        (min_x <= q.x <= max_x) and
        (min_y <= q.y <= max_y) and
        (abs(cross_product(p, q, r)) <= tolerance)
    )


    
def cross_product(p, q, r):
    return (q.y - p.y) * (r.x - q.x) - (q.x - p.x) * (r.y - q.y)


class Graph:
    def __init__(self):
        self.vertices = set()
        self.edges = set()

    def add_vertex(self, point):
        self.vertices.add(point)

    def add_edge(self, point1, point2):
        self.edges.add((point1, point2))
        

def eliminate_duplicate_edges(graph):
    unique_edges = set()

    for edge in graph.edges:
        reversed_edge = (edge[1], edge[0])

        if edge not in unique_edges and reversed_edge not in unique_edges:
            unique_edges.add(edge)

    return unique_edges


def print_graph(graph):

    dict_vertices={}
    vertex_counter=1

    for vertex in graph.vertices:
        dict_vertices[vertex_counter]=vertex
        vertex_counter=vertex_counter+1

    # Vertex Print
    print("V = {")
    for vertex_counter in dict_vertices:
        
        print(f" {vertex_counter}:  ({round(dict_vertices[vertex_counter].x,2)}, {round(dict_vertices[vertex_counter].y,2)})")
    print("}")    

    unique_edges = eliminate_duplicate_edges(graph)
    
    # Edge Print
    print('E = {')
    for index,edge in enumerate(unique_edges):
        for vertex_counter in dict_vertices:
            if edge[0].x == dict_vertices[vertex_counter].x and edge[0].y == dict_vertices[vertex_counter].y:
                
                edge_1=vertex_counter
            if edge[1].x == dict_vertices[vertex_counter].x and edge[1].y == dict_vertices[vertex_counter].y:
                
                edge_2=vertex_counter

        if index == len(unique_edges)-1:
            print(f"  <{edge_1},{edge_2}>")
        else:
            print(f"  <{edge_1},{edge_2}>,")
      
    print("}")



def generate_graph(streets, intersections):

    graph = Graph()

    # Add vertices to the graph from intersection points
    for intersection in intersections:
        graph.add_vertex(intersection)

    # Connect adjacent points on line segments within the same street
    points_between=set()

    for street in streets:
        for line in street.line_List:
            points_between.add(line.src)
            points_between.add(line.dst)
            
    min_distance_intersections_src={}
    min_distance_intersections_dst={}

    for intersection in intersections:
        for street in streets:
            line_List = street.line_List
            for line in line_List:
                
                #for intersection in intersections:
                    
                    if on_segment(line.src, intersection, line.dst):

                        if len(intersections) == 1:
                            if intersection !=line.src:
                                graph.add_edge(intersection,line.src)
                            if intersection !=line.dst:
                                graph.add_edge(intersection,line.dst)
                            graph.add_vertex(line.src)
                            graph.add_vertex(line.dst)
                            continue
                        c=0

                        for intersectio_1 in intersections:
                            if intersectio_1==intersection:
                                continue
                            elif on_segment(line.src,intersectio_1,line.dst):
                                c=c+1

                        if c==0:
                            if intersection !=line.src:
                                graph.add_edge(intersection,line.src)
                            if intersection !=line.dst:
                                graph.add_edge(intersection,line.dst)
                            graph.add_vertex(line.src)
                            graph.add_vertex(line.dst)

                        for intersection_2 in intersections:
                            if intersection_2==intersection:
                                continue
                            
                            c=0
                            if on_segment(line.src,intersection_2,line.dst):

                                if on_segment(intersection,intersection_2,line.src):
                                    
                                    intersection_Edge=Line(intersection,intersection_2)
                                    min_distance_intersections_src[calculate_distance(intersection,intersection_2)]=intersection_Edge
                                    
                                else:
                                    test_otherIntersecitons_lies = False
                                
                                    for intersection_3 in intersections:
                                        
                                        if intersection_3==intersection:
                                            continue
                                        if on_segment(intersection,intersection_3,line.src):
                                            
                                            test_otherIntersecitons_lies=True

                                    if(test_otherIntersecitons_lies == False):
                                        
                                        if intersection !=line.src:
                                            graph.add_edge(intersection,line.src)
                            
                                        graph.add_vertex(line.src)
                            
                            
                                if  on_segment(intersection,intersection_2,line.dst):
                                                            
                                    intersection_Edge=Line(intersection,intersection_2)
                                    min_distance_intersections_dst[calculate_distance(intersection,intersection_2)]=intersection_Edge
                                    
                                
                                else:
                                    test_otherIntersecitons_lies = False
                                
                                    for intersection_3 in intersections:
                                        
                                        if intersection_3==intersection:
                                            continue
                                        if on_segment(intersection,intersection_3,line.dst):
                                        
                                            test_otherIntersecitons_lies=True

                                    if(test_otherIntersecitons_lies == False):
                                        
                                        if intersection !=line.dst:
                                            graph.add_edge(intersection,line.dst)
                            
                                        graph.add_vertex(line.dst)


                    if len(min_distance_intersections_src) > 0:
                        min_key = min(min_distance_intersections_src.keys())
                        min_value = min_distance_intersections_src[min_key]
                        
                        if min_value.src != min_value.dst:
                            graph.add_edge(min_value.src,min_value.dst)

                    if len(min_distance_intersections_dst) > 0:
                        min_key = min(min_distance_intersections_dst.keys())
                        min_value = min_distance_intersections_dst[min_key]
                        
                        if min_value.src != min_value.dst:
                            graph.add_edge(min_value.src,min_value.dst)

                    min_distance_intersections_src.clear()
                    min_distance_intersections_dst.clear()
        
    
    return graph


def is_between(a, b, c):
    
    return min(a, c) <= b <= max(a, c)


def intersect (l1, l2):

    x1, y1 = l1.src.x, l1.src.y
    x2, y2 = l1.dst.x, l1.dst.y
    x3, y3 = l2.src.x, l2.src.y
    x4, y4 = l2.dst.x, l2.dst.y

    det = (x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4)

    if det==0:
        if x2 == x3 and y2 == y3:
            return  Point(x2, y2)
        else:
            return None
    
    xnum = ((x1*y2-y1*x2)*(x3-x4) - (x1-x2)*(x3*y4-y3*x4))
    xden = ((x1-x2)*(y3-y4) - (y1-y2)*(x3-x4))
    xcoor =  xnum / xden

    ynum = (x1*y2 - y1*x2)*(y3-y4) - (y1-y2)*(x3*y4-y3*x4)
    yden = (x1-x2)*(y3-y4) - (y1-y2)*(x3-x4)
    ycoor = ynum / yden

    if (
        is_between(l1.src.x, xcoor, l1.dst.x) and
        is_between(l1.src.y, ycoor, l1.dst.y) and
        is_between(l2.src.x, xcoor, l2.dst.x) and
        is_between(l2.src.y, ycoor, l2.dst.y)
    ):
        
        return Point(xcoor, ycoor)
    else:
        return None
    
    #return Point (xcoor, ycoor)

def main():

    street_list = []
	
    while (True):

        
        line = sys.stdin.readline()

        if line == "":
            break

        command = r'\s*(add|mod|rm|gg)\s+((?:\s*"[a-zA-Z ]+?"\s*?)?)((?:\(\s*-?\s*[0-9]+?\s*,\s*-?\s*[0-9]+?\s*\)\s*?)*)?\s*$'
        
        group_var = re.match(command,line)
        
        if group_var:
            operation = group_var.group(1)
            street_name = group_var.group(2)
            raw_coorlist = group_var.group(3)
            
            if ')(' in raw_coorlist:
                print("Error: The input does not follow the correct format",file= sys.stderr)
                continue    

            if '"(' in line:
                print("Error: The input does not follow the correct format",file= sys.stderr)
                continue

            coorlist = re.findall(r'\(\s*-?\s*[0-9]+?\s*,\s*-?\s*[0-9]+?\s*\)', raw_coorlist)
            
        else:
            print("Error: The input does not follow the correct format", file= sys.stderr)
            continue

        try:
            if operation == 'add':
            
                split_street_name=street_name.split('"')
               
                street_name=split_street_name[1]

                if street_name.isspace():
                    print("Error! Street name is not entered", file= sys.stderr)
                    continue
                
                street_name = str(street_name).lower()

                street_already_exist_check=0

                for street in street_list:
                    if street_name==street.name:
                        street_already_exist_check=1

                if street_already_exist_check==1:
                    print("Error: Street name " + street_name + " already exists!", file= sys.stderr)
                    continue


                if len(coorlist) < 2:
                    print("Error! There should be minimum 2 coordinates", file= sys.stderr)
                    continue
                

                line_list=[]

                for index, point in enumerate(coorlist):

                    if index + 1 < len(coorlist):

                        next_point=coorlist[index +1]
                        values_point1=point.strip("()").split(",")
                        p1=Point(float(values_point1[0]),float(values_point1[1]))
                        values_point2=next_point.strip("()").split(",")
                        p2=Point(float(values_point2[0]),float(values_point2[1]))
                        line=Line(p1,p2)
                        line_list.append(line)

                street=Street(street_name,line_list)
                street_list.append(street)
                            
            
            elif operation == 'mod':
                                
                split_street_name=street_name.split('"')
                street_name=split_street_name[1]
                street_name = str(street_name).lower()    

                if street_name.isspace():
                    print("Error! Street name is not entered", file= sys.stderr)
                    continue

                if len(coorlist) < 2:
                    print("Error! There should be minimum 2 coordinates", file= sys.stderr)
                    continue

                stree_names_list=[]

                for street in street_list:
                    stree_names_list.append(street.name)
         
                if street_name not in stree_names_list:
                    print("Error: Cannot change a street that doesn't exit", file= sys.stderr)
                    continue
                else:
                    index_street_to_delete=0
                    for index, street in enumerate(street_list):
                        if street.name == street_name:
                            index_street_to_delete=index

                    street_list.pop(index_street_to_delete)

                    line_list=[]

                    for index, point in enumerate(coorlist):

                        if index + 1 < len(coorlist):

                            next_point=coorlist[index +1]
                            values_point1=point.strip("()").split(",")
                            p1=Point(float(values_point1[0]),float(values_point1[1]))
                            values_point2=next_point.strip("()").split(",")
                            p2=Point(float(values_point2[0]),float(values_point2[1]))
                            line=Line(p1,p2)
                            line_list.append(line)

                    street=Street(street_name,line_list)
                    street_list.append(street)

                    
            elif operation == 'rm':
                             
                if len(raw_coorlist)!=0 :
                    print("Error: The input does not follow the correct format", file= sys.stderr)
                    continue
                
                split_street_name=street_name.split('"')
                street_name=split_street_name[1]
                street_name = str(street_name).lower() 

                if street_name.isspace():
                    print("Error! Street name is not entered", file= sys.stderr)
                    continue
                
                stree_names_list=[]

                for street in street_list:
                    stree_names_list.append(street.name)

                
                if street_name not in stree_names_list:
                    print("Error: Cannot remove a street that doesn't exit", file= sys.stderr)
                    continue
                else:
                    index_street_to_delete=0
                    for index, street in enumerate(street_list):
                        if street.name == street_name:
                            index_street_to_delete=index

                    street_list.pop(index_street_to_delete)

          
            elif operation == 'gg':

                if len(street_name)>0 :
                    print("Error: The input does not follow the correct format", file= sys.stderr)
                    continue
                
                if len(raw_coorlist)!=0 :
                    print("Error: The input does not follow the correct format", file= sys.stderr)
                    continue

                final_set_intersection=get_Intersection(street_list)

                graph=generate_graph(street_list,final_set_intersection)

                print_graph(graph)

                
        except Exception as exp:
            print("Error: The input does not follow the correct format", file= sys.stderr)
            continue
    
    #print("Finished reading input")
    
    sys.exit(0)


if __name__ == "__main__":
    main()
