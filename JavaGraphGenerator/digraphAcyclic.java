
public class digraphAcyclic {
	
	/**
	 * @param args
	 */
	public static void main(String[] args) {
		int vertices = 50000;
		int edges = 400000;
		String iteration = "e";
		Digraph myGraph = DigraphGenerator.dag(vertices, edges);
		Out output = new Out("/home/graham/workspaceJava/DigraphGen/ConstantVE/cve" + vertices + "x" + edges + iteration + ".txt");
		output.println(myGraph.V());
		output.println(myGraph.E());
		for (int i = 0; i < vertices; i ++)
		{
			for(int edge : myGraph.adj(i))
			{
				output.print(i);
				output.print(' ');
				output.println(edge);
			}
		}
	}
}
